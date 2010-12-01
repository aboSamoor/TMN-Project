#include "rcluster.h"

Rcluster::Rcluster(dmatrix& dists, const int s) : 
	clustered(false), dists(dists){
	
  	_R.parseEvalQ("suppressMessages(require(cluster))");

	Rcpp::NumericMatrix distM(s,s);

	for(int i = 0; i < s; ++i)
		for(int j = 0; j < s; ++j)
			distM(i,j) = dists[i][j];
	
	_R["dists"] = distM;
}

void Rcluster::diana(vector<int>& clusterAssignments, const int k){
	char eval_cstr[45];
	if(clustered)
		sprintf(eval_cstr, "cutree(d, k=%i)", k);
	else
		sprintf(eval_cstr, "d<-diana(dists,TRUE); cutree(d, k=%i)", k);
	cluster(string(eval_cstr), clusterAssignments, k);
}

void Rcluster::agnes(vector<int>& clusterAssignments, const int k){
	char eval_cstr[75];
	if(clustered)
		sprintf(eval_cstr, "cutree(d, k=%i)", k);
	else
		sprintf(eval_cstr, "d<-agnes(dists,TRUE); cutree(d, k=%i)", k);
	cluster(string(eval_cstr), clusterAssignments, k);
}

void Rcluster::mds(){
	_R.parseEval("plot(cmdscale(dists));");
}

void Rcluster::cluster(const string& evalString, vector<int>& clusterAssignments, const int k){
	clusterAssignments.clear();
	clusterAssignments = Rcpp::as< vector<int> >(_R.parseEval(evalString));
	
	rankClusters(clusterAssignments, k);

	clustered = true;
}

void Rcluster::order(vector<int>& orderVector){
	char eval_cstr[30];
	if(clustered)
		sprintf(eval_cstr, "d$order");
	else
		sprintf(eval_cstr, "d<-diana(dists,TRUE); d$order");
	
	orderVector = Rcpp::as< vector<int> >(_R.parseEval(eval_cstr));
}

int Rcluster::pam(vector<int>& clusterAssignments, const int maxK){
	char eval_cstr[200];
	sprintf(eval_cstr, "maxK<-%i; asw<-numeric(maxK); for(k in 2:maxK) asw[k]<-pam(x=dists,k=k,diss=TRUE)$silinfo$avg.width; best.k <- which.max(asw); pam(x=dists, k=best.k, diss=TRUE, cluster.only=TRUE)", maxK);
	
	return pamcluster(string(eval_cstr), clusterAssignments);

}

int Rcluster::pamcluster(const string evalString, vector<int>& clusterAssignments){
	clusterAssignments.clear();
	clusterAssignments = Rcpp::as< vector<int> >(_R.parseEval(evalString));
	int maxK = -1;
	for(vector<int>::iterator it = clusterAssignments.begin(); it < clusterAssignments.end(); ++it)
		if(*it > maxK)
			maxK = *it;

	rankClusters(clusterAssignments, maxK);

	return maxK;
}

void Rcluster::rankClusters(vector<int> &idxs, const int k){
	int ranks[k];  //ranks[i]  -> ith most populated cluster label
	int counts[k]; //counts[i] -> number of structures assigned to label i+1

	for(int i = 0; i < k; ++i) counts[i] = 0;

	for(vector<int>::iterator it = idxs.begin(); it < idxs.end(); ++it)
		counts[(*it)-1]++; //cluster labels are 1-indexed. 

	for(int cl = 0; cl < k; cl++){
		int maxIdx = 0;
		for(int i = 0; i < k; ++i){
			if(counts[maxIdx] < counts[i])
				maxIdx = i;
		}
		ranks[cl] = maxIdx+1; // convert count indices back to 1-indexing
		counts[maxIdx] = 0;
	}

	vector<int> temp(idxs.size());
	for(int rank = 0; rank < k; rank++)
		for(int i = 0; i < idxs.size(); i++)
			if(idxs[i] == ranks[rank])
				temp[i] = rank+1;

	idxs.assign(temp.begin(), temp.end());
}

void Rcluster::psMatrix(const char* outfile){

	if(clustered){
		vector<int> ord;
		order(ord);

		double maxDist = 0.0;
		for(int i = 0; i < ord.size(); ++i){
			for(int j = i; j < ord.size(); ++j){
				if(dists[i][j] > maxDist){
					maxDist = dists[i][j];
				}
			}
		}

		PSMatrix mat(ord.size());
		for(int i = 0; i < ord.size(); ++i) 
			for(int j = 0; j < ord.size(); ++j)
				mat.set(i, j, dists[ord[i]-1][ord[j]-1] / maxDist);
		mat.toPS(outfile);
		
		
	} else {
		printf("ERROR\n");
	}
}

void Rcluster::psMatrix(dmatrix& dists2){

	if(clustered){
		vector<int> ord;
		order(ord);

		double maxDist = 0.0;
		for(int i = 0; i < ord.size(); ++i){
			for(int j = i; j < ord.size(); ++j){
				if(dists[i][j] > maxDist){
					maxDist = dists[i][j];
				}
			}
		}

	   	double maxDist2 = 0.0;
		for(int i = 0; i < ord.size(); ++i){
			for(int j = i; j < ord.size(); ++j){
				if(dists2[i][j] > maxDist2){
					maxDist2 = dists2[i][j];
				}
			}
		}

		PSMatrix mat(ord.size());
		for(int i = 0; i < ord.size(); ++i) {
			for(int j = 0; j < i; ++j)
				mat.set(i, j, dists2[ord[i]-1][ord[j]-1] / maxDist2);
			for(int j = i; j < ord.size(); ++j)
				mat.set(i, j, dists[ord[i]-1][ord[j]-1] / maxDist);
		}
		mat.toPS("mixMatrix.eps");
		
		
	} else {
		printf("ERROR\n");
	}
}
