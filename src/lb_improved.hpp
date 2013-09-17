/**
* Usage (same as LB_Keogh): create the object using the time series you want to match again the
* database and some DTW time constraint parameter. Then repeatedly apply
* the test function on the various candidates. The function returns the
* matching cost with the best candidate so far. By keeping track of when
* the cost was lowered, you can find the nearest neighbor in a database.
*
* Only the l1 norm is used.
*/
class LB_Improved : public  NearestNeighbor{
	public:
	LB_Improved(const vector<double> &  v, int constraint) : NearestNeighbor(v,constraint), lb_keogh(0), full_dtw(0), V(v), buffer(v), mConstraint(constraint),  
	bestsofar(dtw::INF), U(V.size(),0), L(V.size(),0), U2(V.size(),0), L2(V.size(),0) {
		assert(mConstraint>=0);
		assert(mConstraint<static_cast<int>(V.size()));
		computeEnvelope(V,mConstraint,U,L);
	}
	
	void resetStatistics() {
		lb_keogh = 0;
		full_dtw = 0;
	}
	
	
	
	double justlb(const vector<double> & candidate) {
		double error (0.0);
		buffer = candidate;
		for(uint i = 0; i < V.size();++i) {
			if(candidate[i] > U[i]) 
			  error += candidate[i] - U[i];
			else if(candidate[i] < L[i])
			  error += L[i] - candidate[i];
		}
		computeEnvelope(buffer,mConstraint,U2,L2);
		for(uint i = 0; i < V.size();++i) {
				if(V[i] > U2[i]) { 
				  	error += V[i] - U2[i];
				} else if(V[i] < L2[i]) {
				  	error += L2[i] - V[i];
				}
		}
		return error;
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
			}
			if(error < bestsofar) {
				++full_dtw;
				const double trueerror = mDTW.fastdynamic(V,candidate);//,mConstraint,1);
				if(trueerror < bestsofar) bestsofar = trueerror;
			}
		}
		return bestsofar;
	}
	
	/**
	* for plotting purposes
	*/
	string dumpTextDescriptor(const vector<double>  &candidate) {
		//memcpy(&buffer[0], &candidate[0],buffer.size()*sizeof(double));// = candidate;
		buffer = candidate;
		//vector<double> & buffer(candidate);// no need for a copy
		++lb_keogh;
		double error (0.0);
		for(uint i = 0; i < V.size();++i) {
			if(candidate[i] > U[i]) { 
			  error += candidate[i] - U[i];
			  buffer[i] = U[i];
			} else if(candidate[i] < L[i]) {
			  error += L[i] - candidate[i];
			  buffer[i] = L[i];
			} 
			assert((buffer[i]== L[i]) or (buffer[i] == U[i]) or (buffer[i] == candidate[i]));
			
		}
		vector<double> lbimprovedarray;
		computeEnvelope(buffer,mConstraint,U2,L2);
		for(uint i = 0; i < V.size();++i) {
				if(V[i] > U2[i]) { 
				  	error += V[i] - U2[i];
				  	lbimprovedarray.push_back(U2[i]);
				} else if(V[i] < L2[i]) {
				  	error += L2[i] - V[i];
				  	lbimprovedarray.push_back(L2[i]);
				} else 
					lbimprovedarray.push_back(V[i]);

		}
		stringstream ss;
		for(uint k = 0; k <V.size(); ++k) {
			assert((lbimprovedarray[k]== L2[k]) or (lbimprovedarray[k] == U2[k]) or (lbimprovedarray[k] == V[k]));
			assert((buffer[k]== L[k]) or (buffer[k] == U[k]) or (buffer[k] == candidate[k]));
			ss<<k<<" "<<V[k]<<" "<<candidate[k]<<" "<<buffer[k]<<" "<<lbimprovedarray[k]<<" "<<L[k]<<" "<<U[k]<<" "<<L2[k]<<" "<<U2[k]<<endl;
		}
		//string ans;
		//ss>>ans;
		///cout<<ss.str()<<endl;
		return ss.str();
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

