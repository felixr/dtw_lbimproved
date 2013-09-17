/**
 * for debugging purposes
 */
class DimReducedLB_Keogh :  public NearestNeighbor{
    public:
        DimReducedLB_Keogh(const vector<double> &  v, int constraint, int reduced) : NearestNeighbor(v,constraint), lb_keogh(0), full_dtw(0), V(v), mConstraint(constraint),  
        bestsofar(dtw::INF), U(V.size(),0), L(V.size(),0),Ured(reduced,0),Lred(reduced) {
            assert(mConstraint>=0);
            assert(mConstraint<static_cast<int>(V.size()));
            computeEnvelope(V,mConstraint,U,L);
            piecewiseSumReduction(U, Ured);
            piecewiseSumReduction(L, Lred);
        }


        double test(const vector<double> & candidate) {
            vector<double> reducedCandidate(Ured.size());
            piecewiseSumReduction(candidate, reducedCandidate);
            double smallerror (0.0);
            for(uint i = 0; i < Ured.size();++i) {
                if(reducedCandidate[i] > Ured[i]) 
                    smallerror += reducedCandidate[i] - Ured[i];
                else if(reducedCandidate[i] < Lred[i])
                    smallerror += Lred[i] - reducedCandidate[i];
            }
            if(smallerror > bestsofar) return bestsofar;
            ++lb_keogh;
            double error (0.0);
            for(uint i = 0; i < V.size();++i) {
                if(candidate[i] > U[i]) 
                    error += candidate[i] - U[i];
                else if(candidate[i] < L[i])
                    error += L[i] - candidate[i];
            }
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


    private:

        int lb_keogh;
        int full_dtw;
        const vector<double>   V; 
        int mConstraint;
        double bestsofar;
        vector<double>   U,L; 
        vector<double>   Ured,Lred; 

};
