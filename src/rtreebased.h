/**
 * (c) Daniel Lemire, 2008
 * This is a reimplementation of the R-tree based time series indexing approach,
 * as developed by Keogh et al. initially.
 *
 * You must first build and install the spatial index library (http://research.att.com/~marioh/spatialindex/index.html)
 * I built this software with release 1.3.2 - May 23rd, 2008.
 */

#ifndef RTREEBASED
#define RTREEBASED
#include <algorithm>
#include <SpatialIndex.h>
#include <fstream>
#include <iostream>
#include <memory>

#include <sys/stat.h>


class TimeSeriesDB {
public:
    virtual std::vector<double> & readTimeSeries(uint myid) = 0;
};


using namespace std;


/**
 * A time series database
 */
class TimeSeriesTree : public TimeSeriesDB {
public:

    enum {NAIVE,LB_KEOGH,LB_IMPROVED,LB_KEOGH_EARLY,LB_IMPROVED_EARLY,TREE,LINEAR};

    /*
     * constructor to use when the data is already present
     */
    TimeSeriesTree( string  filename );
    ~TimeSeriesTree() { }

    /**
     * constructor to call the first time you create
     * the database
     */
    TimeSeriesTree( string  filename, const uint constraint, const uint dim, const int parent_capacity = 100, const int leave_capacity = 100);

    void close();
    std::vector<double> & readTimeSeries(uint myid);

    void add(const std::vector<double> &  newdata);

    /**
     * return the l_1 distance between the query time series and the nearest neighbor in the
     * database. A slight modification of this code would return the actual nearest neighbor.
     */
    double getNearestNeighborCost(const std::vector<double> &  query, uint type, uint mode = TREE);

    uint getSequentialSearchCandidates()  const;
    uint getNumberOfDTW() const;
    uint getNumberOfCandidates() const;
    void resetStatistics();

    /**
     * return the number of time series stored.
     */
    uint getSize();

protected:
    bool lock();
    void unlock();
    bool good();
private:

    TimeSeriesTree( const  TimeSeriesTree & o) ;

    TimeSeriesTree & operator=(const TimeSeriesTree &o);

    string mFilename;
    uint mConstraint;
    uint mDim;
    std::auto_ptr<SpatialIndex::IStorageManager> diskfile;
    std::auto_ptr<SpatialIndex::StorageManager::IBuffer> file;
    std::auto_ptr<SpatialIndex::ISpatialIndex> tree;
    uint mNextID;
    std::auto_ptr<fstream> mfile;
    uint timeserieslength;
    std::vector<double> outbuffer;
    uint nbrdtw,candidates, nbrqueries;
};



#endif

