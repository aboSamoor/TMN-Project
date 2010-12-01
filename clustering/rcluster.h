#if !defined(RCLUSTER_H)
#define RCLUSTER_H

#include <string>
#include <vector>
#include <RInside.h>
#include <Rcpp.h>
#include "PSMatrix.h"

using namespace std;

class Rcluster{



 private:

	RInside _R;
	
	bool clustered;
	
	void rankClusters(vector<int> &idxs, const int k);

	void cluster(const string& evalString, vector<int>& clusterAssignments, const int k);

	int pamcluster(const string evalString, vector<int>& clusterAssignments);

	vector< vector<int> > dists; //THIS IS FOR A TEST ONLY> REMOVE IT.
	
 public:

	typedef vector< vector <int> > dmatrix;
	
	void psMatrix(const char* outfile);//THIS IS FOR A TEST ONLY> REMOVE IT.
	void psMatrix(vector< vector<int> >& dists2);//THIS IS FOR A TEST ONLY> REMOVE IT.

	/** Instantiates an R session, storing the distance matrix.*/
	Rcluster(dmatrix& dists, const int s);

	void mds();

	void diana(vector<int>& clusterAssignments, const int k);

	void agnes(vector<int>& clusterAssignments, const int k);

	/** Performs k-mediods for k values of 2-maxK. Returns the
	 optimal number of clusters as determined from the silhouette. */
	int pam(vector<int>& clusterAssignments, const int maxK);

	/** Peforms heirarchal clustering (if not done so already). Returns
		the order of the dendrogram. */
	void order(vector<int>& orderVector);
};

#endif
