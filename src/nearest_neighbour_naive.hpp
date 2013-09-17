class NaiveNearestNeighbor :  public NearestNeighbor {
	public:
	NaiveNearestNeighbor(const vector<double> &  v, int constraint) : NearestNeighbor(v,constraint), lb_keogh(0),full_dtw(0),
	 V(v), mConstraint(constraint),  
	bestsofar(dtw::INF) {
	}
	
	double test(const vector<double> & candidate) {
		++lb_keogh;++full_dtw;
		const double trueerror = mDTW.fastdynamic(V,candidate);//,mConstraint,1);
		if(trueerror < bestsofar) bestsofar = trueerror;
		return bestsofar;
	}
	
	
	void resetStatistics() {
		lb_keogh = 0;
		full_dtw = 0;
	}
	double getLowestCost(){return bestsofar;}
	
	int getNumberOfDTW(){return full_dtw;}
	
	
	int getNumberOfCandidates(){return lb_keogh;}
	
	
	private:
	int lb_keogh;
	int full_dtw;
	
	const vector<double>   V; 
	int mConstraint;
	double bestsofar;
	
};

