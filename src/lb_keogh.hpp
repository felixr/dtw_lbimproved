/**
* Usage: create the object using the time series you want to match again the
* database and some DTW time constraint parameter. Then repeatedly apply
* the test function on the various candidates. The function returns the
* matching cost with the best candidate so far. By keeping track of when
* the cost was lowered, you can find the nearest neighbor in a database.
*
* Only the l1 norm is used.
*/
class LB_Keogh :  public NearestNeighbor{
	public:
	LB_Keogh(const vector<double> &  v, int constraint) : NearestNeighbor(v,constraint), lb_keogh(0), full_dtw(0), V(v), mConstraint(constraint),  
	bestsofar(dtw::INF), U(V.size(),0), L(V.size(),0) {
		assert(mConstraint>=0);
		assert(mConstraint<static_cast<int>(V.size()));
		computeEnvelope(V,mConstraint,U,L);
	}
	
	double justlb(const vector<double> & candidate) {
		double error (0.0);
		for(uint i = 0; i < V.size();++i) {
			if(candidate[i] > U[i]) 
			  error += candidate[i] - U[i];
			else if(candidate[i] < L[i])
			  error += L[i] - candidate[i];
		}
		return error;
	}
	double test(const vector<double> & candidate) {
		++lb_keogh;
		double error (0.0);
		for(uint i = 0; i < V.size();++i) {
			if(candidate[i] > U[i]) 
			  error += candidate[i] - U[i];
			else if(candidate[i] < L[i])
			  error += L[i] - candidate[i];
		}
		//cout << "lb keogh = "<<error<<endl;
		if(error < bestsofar) {
			++full_dtw;
			const double trueerror = mDTW.fastdynamic(V,candidate);//,mConstraint,1);
			if(trueerror < bestsofar) bestsofar = trueerror;
		}
		return bestsofar;
	}
	
	
	int getNumberOfDTW(){return full_dtw;}
	
	
	int getNumberOfCandidates(){return lb_keogh;}
	
	
	double getLowestCost(){return bestsofar;}
	
	void resetStatistics() {
		lb_keogh = 0;
		full_dtw = 0;
	}
	
	private:
	
	int lb_keogh;
	int full_dtw;
	const vector<double>   V; 
	int mConstraint;
	double bestsofar;
	vector<double>   U; 
	vector<double>   L; 
	
};


class LB_KeoghEarly :  public NearestNeighbor{
	public:
	LB_KeoghEarly(const vector<double> &  v, int constraint) : NearestNeighbor(v,constraint), lb_keogh(0), full_dtw(0), V(v), mConstraint(constraint),  
	bestsofar(dtw::INF), U(V.size(),0), L(V.size(),0) {
		assert(mConstraint>=0);
		assert(mConstraint<static_cast<int>(V.size()));
		computeEnvelope(V,mConstraint,U,L);
	}
	
	double test(const vector<double> & candidate) {
		++lb_keogh;
		double error (0.0);
		for(uint i = 0; i < V.size();++i) {
			if(candidate[i] > U[i]) 
			  error += candidate[i] - U[i];
			else if(candidate[i] < L[i])
			  error += L[i] - candidate[i];
			if(error > bestsofar) return bestsofar;
		}
			++full_dtw;
			const double trueerror = mDTW.fastdynamic(V,candidate);//,mConstraint,1);
			if(trueerror < bestsofar) bestsofar = trueerror;
		return bestsofar;
	}
	
	
	int getNumberOfDTW(){return full_dtw;}
	
	
	int getNumberOfCandidates(){return lb_keogh;}
	
	
	double getLowestCost(){return bestsofar;}
	
	void resetStatistics() {
		lb_keogh = 0;
		full_dtw = 0;
	}
	
	private:
	
	int lb_keogh;
	int full_dtw;
	const vector<double>   V; 
	int mConstraint;
	double bestsofar;
	vector<double>   U; 
	vector<double>   L; 
	
};

