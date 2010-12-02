#include "PSMatrix.h"

#define DEBUG true

PSMatrix::PSMatrix(const int dim) : xx(dim), yy(dim), ll_x(0), ll_y(0), ur_x(BBOX), ur_y(BBOX) {
	//we will draw the matrix in an 8in by 8in bounding box
	data.resize(dim, vector<double>(dim, 0.0));
	labels.resize(dim, 1.0); // 1 is white
	cell_w = cell_h = BBOX / dim;
}

PSMatrix::PSMatrix(const int dim_x, const int dim_y) : xx(dim_x), yy(dim_y), ll_x(0), ll_y(0), ur_x(BBOX), ur_y(BBOX) {
	//we will draw the matrix in an 8in by 8in bounding box
	data.resize(dim_x, vector<double>(dim_y, 0.0));
	cell_w = BBOX / dim_x;
	cell_h = BBOX / dim_y;
}

PSMatrix::PSMatrix(vector< vector<double> > &data) : ll_x(0), ll_y(0), ur_x(BBOX), ur_y(BBOX), data(data) {
	//TODO: check that data is square (dim x dim)?
	xx = data.size();
	yy = data.size() > 0 ? data.front().size() : 0;

	//data[rows][columns]
	cell_w = BBOX / xx;
	cell_h = BBOX / yy;
}

bool PSMatrix::writeMatrix(std::ofstream &os){
	for(int y = 0; y < yy; ++y)
		for(int x = 0; x < xx; ++x){ // for upper triangular, initialize as x = y
			printColor(x, y, os);
			printSquare(x, y, os);
		}
	return os.good();
}

void PSMatrix::printColor(const int x, const int y, std::ofstream &os){
	if(x >= xx || y >= yy)
		os << 0 << " setgray" << std::endl; // 0 is black, 1 is white
	else if (x == y)
		os << labels[x] << " 1 1 sethsbcolor" << std::endl; 
	else
		os << (1-data[x][y]) << " setgray" << std::endl; // 0 is black, 1 is white
}

void PSMatrix::printSquare(const int x, const int y, std::ofstream &os){
	os << x*cell_w << " " // x
	   << BBOX-(y+1)*cell_h << " " // y
	   << cell_w << " "   // width
	   << cell_h          // height
	   << " rectfill" << std::endl; 
}

bool PSMatrix::writeBorder(std::ofstream &os){
	//TODO: implement body
	return os.good();
}

bool PSMatrix::writeEPSHead(std::ofstream &os){
	os << "%!PS-Adobe-3.0 EPSF-3.0\n" << "%%BoundingBox: " 
	   << ll_x << " " << ll_y << " " << ur_x << " " << ur_y << std::endl;

	time_t theTime;
	os << "%%BeginComments\n"
	   << "%%Creator: Zfold\n"
	   << "%%Title: Cluster Closeness\n"
	   << "%%Document-Fonts: \n"
	   << "%%EndComments\n";
	return os.good();
}

void PSMatrix::toPS(std::ofstream &out){
	writeEPSHead(out);
	writeBorder(out);
	writeMatrix(out);
}

void PSMatrix::toPS(const char* filename){
	std::ofstream out(filename);
	toPS(out);
}

void PSMatrix::set(const int x, const int y, const double value){
	if(x < data.size())
		if(y < data[x].size())
			data[x][y] = value;
}

void PSMatrix::label(const int i, const double val){
	labels[i] = val;
}
