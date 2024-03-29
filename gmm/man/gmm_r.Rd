% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/gmm.R
\name{gmm_r}
\alias{gmm_r}
\title{Expectation Maximization for 1-D Gaussian Mixture}
\usage{
gmm_r(y, init, tol = 1e-05, max_steps = 1000)
}
\arguments{
\item{y}{Gaussian Mixture observations}

\item{init}{list of initial values
\describe{
\item{}{\code{mu}: mixture means}
\item{}{\code{sigma_sq}: mixture variances}
\item{}{\code{class_prob}: mixture weights}
}}

\item{tol}{stopping rule tolerance for change in objective function}

\item{max_steps}{stopping rule maximum number of steps}
}
\value{
named list with the following components:
\describe{
\item{}{\code{mu}: mixture means}
\item{}{\code{sigma_sq}: mixture variances}
\item{}{\code{class_prob}: mixture weights}
}
}
\description{
Expectation Maximization for 1-D Gaussian Mixture
}
\examples{
# Simulate Data
n <- 1000
K <- 3
class_prob <- c(0.5, 0.3, 0.2)
mu <- rnorm(3)
sigma_sq <- 1/rgamma(3, 1,1)
class_label <- sample(1:3, n, replace = T, prob = class_prob)
y <- purrr::map_dbl(1:n, ~{
  rnorm(1, mu[class_label[.x]], sd = sqrt(sigma_sq[class_label[.x]]))
})
# Initialize Parameters
init <- list(mu = runif(3),
             sigma_sq = runif(3),
             class_prob = class_prob)
# Run EM
fit <- gmm_em(y, init)
}
