
%module lbimproved
%{        

#include "dtw.h"

#include "nearest_neighbour.hpp"
#include "nearest_neighbour_naive.hpp"
#include "lb_improved.hpp"
#include "lb_improved_early.hpp"
#include "lb_keogh.hpp"
#include "lb_keogh_dimreduced.hpp"

#include "rtreebased.h"

%}

%include "typemaps.i"
%include "std_string.i"
%include "cstring.i"
%include "std_vector.i"
namespace std {
    %template(vectord) vector<double>;
};
%include "dtw.h"
%include "rtreebased.h"
