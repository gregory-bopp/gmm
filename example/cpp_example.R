library(purrr)
library(ggplot2)
library(magrittr)
library(dplyr)
library(gmm)
library(here)

set.seed(2)

data_dir <- here("example", "data")

# Simulate Data -----------------------------------------------------------
n <- 500
K <- 3
class_prob <- c(0.5, 0.3, 0.2)
mu <- c(-1, 0, 1)
sigma_sq <- c(1, 3, 1)
class_label <- sample(1:K, n, replace = T, prob = class_prob)
y <- purrr::map_dbl(1:n, ~{
  rnorm(1, mu[class_label[.x]], sd = sqrt(sigma_sq[class_label[.x]]))
})

# Initialize Parameters ---------------------------------------------------
init <- list(mu = runif(K),
             sigma_sq = 1/rgamma(K,1,1),
             class_prob = rep(1/K, K))


# Run EM ------------------------------------------------------------------
# R-version
fit_r <- gmm_r(y, init, max_steps = 1000)

# C++ Version
fit_cpp <- gmm_cpp(y = y,
             mu = init$mu,
             sigma_sq = init$sigma_sq,
             class_prob = init$class_prob,	      
             max_steps = 1000)

# Confirm that results match
map2(fit_r, fit_cpp, ~{max(abs(.x - .y))})
