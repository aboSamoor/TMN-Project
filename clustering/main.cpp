#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include "main.h"

using namespace std;

#define LINE_LIMIT 5000
#define KMEANS

int cluster(vector< vector<int> > &dists, vector<string> &labels, 
			const int n, const int maxK, string &output)
{
	int numClusters;
	Rcluster r(dists, n);
	vector<int> idxs;
	idxs.reserve(n);

#ifdef KMEANS
	numClusters = r.pam(idxs, maxK);

	/* use maxDist to scale the color [0,1] for eps output */
	double maxDist = 0.0;
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			if(dists[i][j] > maxDist)
				maxDist = (double)dists[i][j];

	/* calculate the largest cluster, and how good our guesses are */
	vector<int> counts(numClusters+1, 0);
	vector<int> nReal(numClusters+1, 0);
	for (int i = 0; i < idxs.size(); i++) {
		counts[idxs[i]]++;
		nReal[idxs[i]] += labels[i].compare("T");
	}
	for(int i = 0; i < counts.size(); i++){
		cout << "cluster" << i << " size: " << counts[i] <<  " # real: " 
			 << nReal[i] << endl;
	}

	/* Rearrange the points so that cluster members are together.
	   Also add a classification label along the diagonal */
	PSMatrix mat(idxs.size());
	int idx = 0;
	vector<int> order(idxs.size(), 0);
	for (int cNum = 1; cNum <= numClusters; cNum++) {
		for (int i = 0; i < idxs.size(); ++i) {
			if (idxs[i] == cNum) {
				mat.label(idx, labels[i].compare("T") ? .6 : 0); //U=Blue, T=Red
				order[i] = idx++;
			}
		}
	}

	/* Input actual matrix values, normalized */
	for(int i = 0; i < idxs.size(); ++i) 
		for(int j = 0; j < idxs.size(); ++j)
			mat.set(order[i],order[j], dists[i][j] / maxDist);
	mat.toPS(output.c_str());
#else
	r.diana(idxs, maxK);
	r.psMatrix(output.c_str());
#endif
	return numClusters;
}

int main(int argc, char** argv)
{
	/* dissimilarity matrix */
	vector< vector<int> > dists;
	vector< string > labels;

	string label; 
	int dist;
	
	string file_name;
	int n;
	string output;

	/* grab filename matrix size from standard input */
	cout << "filename: ";
	cin >> file_name;
	cout << "n: ";
	cin >> n;
	cout << file_name << ", " << n << endl;

	dists.resize(n, vector<int>(n, 0));
	labels.resize(n);

	/* open a file copy */
   ifstream inFile(file_name.c_str());
   if (!inFile) {
      cerr << "unable to open input file: "
		   << file_name << endl;
      return -1;
   }

   /* (n+1)x(n) matrix */
   for (int i = 0; i < n; ++i) {
	   /* exract label (T/R = trackmenot/real) */
	   inFile >> label;
	   labels[i] = label;
#ifdef DEBUG
	   cout << label << "\t";
#endif
	   /* n tokens left in line (dissimilarities) */
	   for (int j = 0; j < n; ++j) {
		   inFile >> dist;
		   dists[i][j] = dist;
#ifdef DEBUG
		   cout << dists[i][j] << "\t";
#endif
	   }
#ifdef DEBUG
	   cout << endl;
#endif
   }


   /* this is a hack: necessary because similarity is backwards in
	   our calculations. 0 should be identical, not 1000000000 */
   for(int i = 0; i < n; ++i) {
	   for (int j = 0; j < n; ++j) {
		   if(i != j) {
			   dists[i][j] = 1000000000 - dists[i][j];
		   }
	   }
   }
   
   cout << "Output filename (.eps): ";
   cin >> output;

   cluster(dists, labels, n, n-1, output);

   return 0;
}
