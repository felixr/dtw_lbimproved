class LB_ImprovedEarly : public  NearestNeighbor{
	public:
	LB_ImprovedEarly(const vector<double> &  v, int constraint) : NearestNeighbor(v,constraint), lb_keogh(0), full_dtw(0), V(v), buffer(v), mConstraint(constraint),  
	bestsofar(dtw::INF), U(V.size(),0), L(V.size(),0), U2(V.size(),0), L2(V.size(),0) {
		assert(mConstraint>=0);
		assert(mConstraint<static_cast<int>(V.size()));
		computeEnvelope(V,mConstraint,U,L);
	}
	
	void resetStatistics() {
		lb_keogh = 0;
		full_dtw = 0;
	}

	double test(const vector<double>  &candidate) {
		//memcpy(&buffer[0], &candidate[0],buffer.size()*sizeof(double));// = candidate;
		//buffer = candidate;
		//vector<double> & buffer(candidate);// no need for a copy
		++lb_keogh;
		double error (0.0);
		for(uint i = 0; i < V.size();++i) {
			const double & cdi (candidate[i]); 
			if(cdi > U[i]) { 
			  error += cdi - (buffer[i] = U[i]);
			} else if(cdi < L[i]) {
			  error += (buffer[i] = L[i]) - cdi;
			} else buffer[i] = cdi;
			if(error>bestsofar) return bestsofar;
		}
		if(error < bestsofar) {
			computeEnvelope(buffer,mConstraint,U2,L2);
			//env.compute(buffer,mConstraint,U2,L2);
			for(uint i = 0; i < V.size();++i) {
				if(V[i] > U2[i]) { 
				  	error += V[i] - U2[i];
				} else if(V[i] < L2[i]) {
				  	error += L2[i] - V[i];
				}
				if(error>bestsofar) return bestsofar;

			}
			if(error < bestsofar) {
				++full_dtw;
				const double trueerror = mDTW.fastdynamic(V,candidate);//,mConstraint,1);
				if(trueerror < bestsofar) bestsofar = trueerror;
			}
		}
		return bestsofar;
	}

	
	int getNumberOfDTW(){return full_dtw;}
	
	int getNumberOfCandidates(){return lb_keogh;}
	
	double getLowestCost(){return bestsofar;}
	private:
	
	int lb_keogh;
	int full_dtw;
	const vector<double>   V;
	vector<double> buffer;  
	int mConstraint;
	double bestsofar;
	vector<double>   U;  
	vector<double>   L;  
	vector<double>   U2;  
	vector<double>   L2;  

};
