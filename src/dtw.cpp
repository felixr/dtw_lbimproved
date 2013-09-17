#include "dtw.h"

using namespace std;

/*
 * hardcoded l1 norm (for speed)
 */
double dtw::fastdynamic(const vector<double> &  v, const vector<double> & w) {
        if(! fast) return dynamic(v,w,mConstraint,1);
        assert(static_cast<int>(v.size()) == mN);
        assert(static_cast<int>(w.size()) == mN);
        assert(static_cast<int>(mGamma.size()) == mN);
        double Best(INF);
        for (int i = 0; i < mN; ++i) {
            assert(static_cast<int>(mGamma[i].size()) == mN);
            for (int j = max(0,i-mConstraint); j < min(mN,i+mConstraint+1); ++j) {
                Best = INF;
                if(i>0) Best = mGamma[i-1][j];
                if(j>0) Best = min(Best, mGamma[i][j-1]);
                if( (i > 0) && (j > 0) )
                    Best = min(Best, mGamma[i-1][j-1]);
                if((i==0) && (j==0))
                    mGamma[i][j] = fabs(v[i]-w[j]);
                else
                    mGamma[i][j] = Best + fabs(v[i]-w[j]);
            }
        }
        return mGamma[mN-1][mN-1];
    }

double dtw::dynamic(const vector<double> &  v, const vector<double> & w,
        int constraint, int p) {
    //cout<<"dtw "<<constraint<<" "<<v.size()<<" "<<w.size()<<" "<<p<<endl;
    assert(v.size() == w.size());
    int n ( v.size() );
    vector<vector<double> > gamma(n, vector<double>(n,0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(abs(i-j) > constraint) {
                gamma[i][j] = INF;
                continue;
            }
            vector<double> previous(0);
            if(i>0) previous.push_back(gamma[i-1][j]);
            if(j>0) previous.push_back(gamma[i][j-1]);
            if( (i > 0) && (j > 0) )
                previous.push_back(gamma[i-1][j-1]);
            double smallest = *min_element(previous.begin(),previous.end());
            if (p != INF)
                gamma[i][j] = pow(fabs(v[i]-w[j]),p)+smallest;
            else
                gamma[i][j] = max(fabs(v[i] - w[j]), smallest);

        }
    }
    if(p != INF)
        return pow(gamma[n-1][n-1],1.0/p);
    else
        return gamma[n-1][n-1];
}

/**
 * You do not need this function, used by LB_Keogh and LB_Improved later.
 */
void computeEnvelope(const vector<floattype> & array, uint constraint, vector<floattype> & maxvalues, vector<floattype> & minvalues) {
    uint width = 1+  2 * constraint;
    deque<int> maxfifo, minfifo;
    maxfifo.push_back(0);
    minfifo.push_back(0);
    for(uint i = 1; i < array.size(); ++i) {
        if(i >=constraint+1) {
            maxvalues[i-constraint-1] = array[maxfifo.front()];
            minvalues[i-constraint-1] = array[minfifo.front()];
        }
        if(array[i] > array[i-1]) { //overshoot
            maxfifo.pop_back();
            while(maxfifo.size() > 0) {
                if(array[i] <= array[maxfifo.back()]) break;
                maxfifo.pop_back();
            }
        } else {
            minfifo.pop_back();
            while(minfifo.size() > 0) {
                if(array[i] >= array[minfifo.back()]) break;
                minfifo.pop_back();
            }
        }
        maxfifo.push_back(i);
        minfifo.push_back(i);
        if(i==  width+maxfifo.front()) maxfifo.pop_front();
        else if(i==  width+minfifo.front()) minfifo.pop_front();
    }
    for(uint i = array.size(); i <= array.size() + constraint; ++i) {
        maxvalues[i-constraint-1] = array[maxfifo.front()];
        minvalues[i-constraint-1] = array[minfifo.front()];
        if(i-maxfifo.front() >= width) maxfifo.pop_front();
        if(i-minfifo.front() >= width) minfifo.pop_front();
    }
}


void piecewiseSumReduction(const vector<floattype> & array, vector<floattype> & out) {
    // the length of out gives out the desired output length
    assert(out.size()>0);
    const uint sizeofpieces = array.size()/out.size();
    assert(sizeofpieces>0);
    //sum_up<floattype> s;
    for(uint k = 0; k<out.size()-1; ++k) {
        //s.reset();
        out[k] = 0;
        for(uint j = k*sizeofpieces; j < (k+1)*sizeofpieces; ++j)
            out[k] += array[j];
    }
    uint k=out.size()-1;
    out[k] = 0;
    for(uint j = k*sizeofpieces; j < array.size(); ++j)
        out[k] += array[j];
}


// only used for unit testing
double l1diff(const vector<double> &  v, const vector<double> & w) {
    double ans(0);
    for(uint k = 0; k<v.size(); ++k)
        ans+=abs(v[k]-w[k]);
    return ans;
}
