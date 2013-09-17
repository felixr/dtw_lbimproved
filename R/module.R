
lb_lib <- Module("lbimproved", PACKAGE="lbimproved")


TimeSeriesTree <- function(file_name, constraint=0, dimension=5, 
                           parent_capacity= 100, leaf_capacity=100) {
    tree = new(lb_lib$TimeSeriesTree)
    initialize(file_name, constraint, dimension, parent_capacity, leaf_capacity)
}
