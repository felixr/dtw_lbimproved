#ifndef _NEAREST_NEIGHBOUR_HPP
#define _NEAREST_NEIGHBOUR_HPP 

class NearestNeighbor {
	public:
		NearestNeighbor(const vector<double> &  v, int constraint) : mDTW(v.size(),constraint) {}
		virtual double test(const vector<double> & candidate) {return 0;}//= 0;
		virtual double getLowestCost() {return 0;}
		virtual ~NearestNeighbor() {};
		virtual int getNumberOfDTW(){return 0;}
		virtual int getNumberOfCandidates(){return 0;}
		dtw mDTW;
};

#endif
