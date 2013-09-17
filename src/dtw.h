/**
 * (c) Daniel Lemire, 2008
 *This C++ library implements fast nearest-neighbor retrieval under the dynamic time warping (DTW).
 * This library includes the dynamic programming solution, a fast implementation of the LB_Keogh
 *  lower bound, as well as a improved lower bound called LB_Improved. This library was used to show
 *  that LB_Improved can be used to retrieve nearest neighbors three times on several data sets
 * including random walks time series or shape time series.
 *
 * See the classes LB_Keogh and LB_Improved for usage.
 *
 * Time series are represented using STL vectors.
 */

#ifndef DTW
#define DTW

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <deque>
#include <sstream>
#include <assert.h>

typedef double floattype;
typedef unsigned int uint;

using namespace std;

/**
 * You do not need this class, used by LB_Keogh and LB_Improved later.
 * It computes the DTW using dynamic programmming.
 */
class dtw {
public:
    dtw(uint n, uint constraint): mGamma(n,vector<double>(n,INF)),mN(n),mConstraint(constraint) {}

    enum { verbose = false, vverbose = false, INF = 100000000, fast=true };
    double fastdynamic(const vector<double> &  v, const vector<double> & w);
    double dynamic(const vector<double> &  v, const vector<double> & w,
                                 int constraint=INF, int p=2);

protected:

    static inline double max (double x, double y ) { return x > y ? x : y; }
    static inline double min (double x, double y ) { return x < y ? x : y; }

    vector<vector<double> > mGamma;
    int mN, mConstraint;
};


void computeEnvelope(const vector<floattype> & array, uint constraint,
        vector<floattype> & maxvalues, vector<floattype> & minvalues);

void piecewiseSumReduction(const vector<floattype> & array, vector<floattype> & out);

#endif


