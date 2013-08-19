# LBImproved C++ Library

**Author:** Daniel Lemire 

**License:** GPL v3

This library comes in the form of one short C++ header file. The documentation
is in the C++ comments and in this file.

This C++ library implements fast nearest-neighbor retrieval under the dynamic
time warping (DTW). This library includes the dynamic programming solution, a
fast implementation of the LB_Keogh lower bound, as well as a improved lower
bound called LB_Improved. This library was used to show that LB_Improved can be
used to retrieve nearest neighbors three times faster on several data sets
including random walks time series or shape time series.

The library assumes that your time series have the same length. If they don't, you need to preprocess them (perhaps with linear interpolation) so that they have the same length.

**Reference:**
- Daniel Lemire, Faster Retrieval with a Two-Pass Dynamic-Time-Warping Lower Bound, Pattern Recognition Volume 42, Issue 9, September 2009, Pages 2169-2180. http://arxiv.org/abs/0811.3301
- Original source: http://code.google.com/p/lbimproved/

## Key features

1) Fast Dynamic Time Warping nearest neighbor retrieval.
2) Persistence
3) External-memory: you need only a constant amount of RAM


## PREREQUISITES

- You must first build and install the spatial index library 
  (http://research.att.com/~marioh/spatialindex/index.html)
  I built this software with release 1.3.2 - May 23rd, 2008.
- While not strictly necessary, SWIG (http://www.swig.org) is strongly recommended.
  I interact with the library using swig and python.
- If you are using SWIG, Python is recommended. I have used Python 2.5.

## OPERATING SYSTEM

I built and ran this software with Mac OS 10.4. It also builds under Linux
if you have Python 2.5 and swig installed. It should be possible to
use  any other Unix-like operating system, or even Windows. 

## BUILD

type "make"


## TESTING

type "python unitesting.py"

##USAGE

```python
import dtw
constraint = 0.1
n = 128
c = int(constraint*n)
rtree = dtw.TimeSeriesTree("mytmpfile.bin",c,reducdim)
# randomwalk(n) return a size n array
for i in xrange(1000):
    rtree.add(randomwalk(n))
x = randomwalk(n)
for mode in [rtree.LINEAR, rtree.TREE]:
for algo in [ rtree.NAIVE,rtree.LB_KEOGH, rtree.LB_IMPROVED]:
    rtree.getNearestNeighborCost(x,algo,mode) 
rtree.close()
# to reopen the tree, just do this:
rtree = dtw.TimeSeriesTree("mytmpfile.bin")
```

```c
#include "dtw.h"
(...)
// compute the DTW between two vectors:
double fd = mDTW.fastdynamic(x,y);
// to seek a nearest neighbor, construct a LB_Improved object:
LB_Improved pruner = new LB_Improved(targetvector, constraint);// constraint can be 10% of vector length
// then repeatedly call the test method, which returns the best distance so far
double bestdistancesofar = pruner.test(candidate);
// the test method is typically much cheaper than a full DTW
```
