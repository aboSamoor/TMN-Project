#if !defined(PS_MAT)
#define PS_MAT

#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

#define BBOX 572.0
using namespace std;
class PSMatrix{

 public:

	/** Makes a matrix of dimensions data.size() x data.front.size()*/
	PSMatrix(vector< vector<double> > &data);

	/** Makes a dim x dim square matrix. */
	PSMatrix(const int dim);

	/** Maxes a dim_x x dim_y square (in shape) matrix. */
	PSMatrix(const int dim_x, const int dim_y);

	/** Assigns value to location (x,y) in the matrix. */
	void set(const int x, const int y, const double value);

	void label(const int i, const double val);

	void toPS(const char* filename);
	void toPS(std::ofstream& out);

 private:

	int                         xx;

	int                         yy;

	int                         ll_x;

	int                         ll_y;

	int                         ur_x;

	int                         ur_y;
	
	double                      line_w;

	double                      cell_w;

	double                      cell_h;

	vector< vector<double> >    data;
	vector<double>              labels;

	bool writeEPSHead(std::ofstream &os);
	bool writeBorder(std::ofstream &os);
	bool writeMatrix(std::ofstream &os);

	void printColor(const int x, const int y, std::ofstream &os);
	void printSquare(const int x, const int y, std::ofstream &os);
};

#endif // PS_MAT defined
