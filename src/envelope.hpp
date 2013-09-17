/**
* this class is not used?
*/
class Envelope {
	public:
	Envelope() : maxfifo(), minfifo(){}
	virtual ~Envelope() {}
	void compute(const vector<floattype> & array, uint constraint, vector<floattype> & maxvalues, vector<floattype> & minvalues) {
	    const uint width = 1 +  2 * constraint;
	    maxfifo.clear();minfifo.clear();
        for(uint i = 1; i < array.size(); ++i) {
          if(i >=constraint+1) {
            maxvalues[i-constraint-1] = array[maxfifo.size()>0 ? maxfifo.front():i-1];
            minvalues[i-constraint-1] = array[minfifo.size()>0 ? minfifo.front(): i-1];
          }
          if(array[i] > array[i-1]) { //overshoot
            minfifo.push_back(i-1);
            if(i ==  width+minfifo.front()) minfifo.pop_front();
            while(maxfifo.size() > 0) {
              if(array[i] <= array[maxfifo.back()]) {
                if (i==  width+maxfifo.front()) maxfifo.pop_front();
                break;
              }
              maxfifo.pop_back();
            }
          } else {
            maxfifo.push_back(i-1);
            if (i==  width+maxfifo.front()) maxfifo.pop_front();
            while(minfifo.size() > 0) {
              if(array[i] >= array[minfifo.back()]) {
                if(i==  width+minfifo.front()) minfifo.pop_front();
                break;
              }
              minfifo.pop_back();
            }
          }  
        }
        for(uint i = array.size(); i <= array.size() + constraint; ++i) {
          	if(maxfifo.size()>0) {
          		maxvalues[i-constraint-1] = array[maxfifo.front()];
        		if(i-maxfifo.front() >= width) {
        			maxfifo.pop_front();}
          	} else {
          		maxvalues[i-constraint-1] = array[array.size()-1];
          	}
          	if(minfifo.size() > 0) {
          		minvalues[i-constraint-1] = array[minfifo.front()];
          		if(i-minfifo.front() >= width) {
          			minfifo.pop_front();}
          	} else {
          		minvalues[i-constraint-1] = array[array.size()-1];
          	}
        }
 }
        deque<int> maxfifo, minfifo;


};

