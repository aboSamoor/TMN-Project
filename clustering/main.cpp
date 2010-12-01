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
			const int n, const int maxK)
{
	int numClusters;
	Rcluster r(dists, n);
	vector<int> idxs;
	idxs.reserve(n);

#ifdef KMEANS
	numClusters = r.pam(idxs, maxK);

	double maxDist = 0.0;
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			if(dists[i][j] > maxDist)
				maxDist = (double)dists[i][j];

	PSMatrix mat(idxs.size());
	int idx = 0;
	vector<int> order(idxs.size(), 0);
	for(int cNum = 1; cNum <= numClusters; cNum++)
		for(int i = 0; i < idxs.size(); ++i){
			if(idxs[i] == cNum) {
				mat.label(idx, labels[i].compare("T") ? .6 : 0);
				order[i] = idx++;
			}
		}
	for(int i = 0; i < order.size(); ++i)
		printf("%i %i\n", order[i], idxs[i]);

	for(int i = 0; i < idxs.size(); ++i) 
		for(int j = 0; j < idxs.size(); ++j)
			mat.set(order[i],order[j], dists[i][j] / maxDist);
	mat.toPS("cluster_map.eps");
#else
	r.diana(idxs, maxK);
	r.psMatrix("test.eps");
#endif
	return numClusters;
}

int main(int argc, char** argv)
{
	//dissimilarity matrix
	vector< vector<int> > dists;
	vector< string > labels;

	string label; 
	int dist;

	string file_name;
	int n;

	//grab filename from standard input
	cout << "filename: ";
	cin >> file_name;
	cout << "n: ";
	cin >> n;
	cout << file_name << ", " << n << endl;

	dists.resize(n, vector<int>(n, 0));
	labels.resize(n);

   //open a file copy
   ifstream inFile(file_name.c_str());
   if (!inFile) {
      cerr << "unable to open input file: "
		   << file_name << endl;
      return -1;
   }

	//while lines to read
   for (int i = 0; i < n; ++i) {
	   //exract label (T/R = trackmenot/real)
	   inFile >> label;
	   cout << label << "\t";
	   labels[i] = label;

	   //n tokens left in line (dissimilarities)
	   for (int j = 0; j < n; ++j) {
		   inFile >> dist;
		   dists[i][j] = dist;
		   cout << dists[i][j] << "\t";
	   }
	   cout << endl;
   }

   cluster(dists, labels, n, n-1);

   return 0;
}
