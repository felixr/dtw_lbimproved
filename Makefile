OSNAME = $(shell uname | tr "[:upper:]" "[:lower:]")
SHAREDNAME=$(shell if [  $(OSNAME) = "darwin" ]; then echo -n "   -bundle -flat_namespace -undefined suppress"; else echo -n "-shared";fi )

LIBSPATIALINDEX_INCLUDES=$(shell pkg-config --cflags-only-I libspatialindex)
PYTHON_INCLUDES=$(shell pkg-config --cflags-only-I python)

CXXFLAGS=-fPIC -O2 $(LIBSPATIALINDEX_INCLUDES) $(PYTHON_INCLUDES) -std=c++0x
LIBS=/usr/lib/libspatialindex.a
LIBS+=$(shell pkg-config --libs python) 
RLIBS=+=-L/usr/lib/R/lib -lR

all: _dtw.so 

package:
	zip -9 lbimproved_`date +%Y-%m-%d`.zip dtw.h Makefile rtreebased.h querystrategy.h unitesting.py timeseries.i unittesting.cpp README data/*py data/READ*

timeseries_wrap.o: timeseries.i
	# this assumes you have swig on your system
	swig -Wall -c++ -python timeseries.i
	$(CXX) $(CXXFLAGS) -c  timeseries_wrap.cxx

timeseries_rwrap.so: timeseries.i
	swig -Wall -c++ -r -o timeseries_rwrap.cxx timeseries.i
	$(CXX) $(CXXFLAGS) -I /usr/share/R/include/ -shared timeseries_rwrap.cxx -o $@
	# PKG_LIBS="timeseries_rwrap.cxx" R CMD SHLIB timeseries_rwrap.cxx


_dtw.so: timeseries_wrap.o Makefile
	$(CXX) $(SHAREDNAME) timeseries_wrap.o -o _dtw.so  $(LIBS)

clean:
	rm -f _dtw.so timeseries_wrap.o timeseries_wrap.cxx timeseries_rwrap.cxx *.so

unittesting: unittesting.cpp dtw.h rtreebased.h
	g++ -g3 -Wall -Wold-style-cast  -Woverloaded-virtual -o unittesting unittesting.cpp
