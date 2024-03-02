# Gaussian Mixture Model
One goal of this project was to become more familiar with [R internals](https://github.com/hadley/r-internals). This package contains two simple implementations of an EM algorithm to fit a [Gaussian mixture model](https://en.wikipedia.org/wiki/Mixture_model#Gaussian_mixture_model), one in R and one with (stl only) c++. Finally, the c++ is ported into R without the use of Rcpp. 

See [mclust](https://cran.r-project.org/web/packages/mclust/vignettes/mclust.html) for a more robust R implementation for fitting Gaussian mixture models.


# R Installation
```
devtools::install_github("gregory-bopp/gaussian_mixture_model/gmm")
```

# 1. R Implementation
See `example/r_example.R` for an example of simulating data from a GMM and fit using the gmm EM implementation.

# 2. Standalone C++ Implementation
1. Simulate GMM data with `example/example.R`. Data and initial values are stored in `example/data/`
2. Build gmm cpp program with `make` from `gmmcpp`
3. Run cpp implementation of EM algorithm to find ML estimates for GMM with 

```
$ ./gmmcpp/bin/gmm -v -i ../example/data/init.csv -o ../example/data/fit.csv ../example/data/data.csv 
```

# R wrapper of C++ Implementation
See `example/cpp_example.R`




