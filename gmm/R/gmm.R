#' Expectation Maximization for 1-D Gaussian Mixture
#'
#' @param y Gaussian Mixture observations
#' @param init list of initial values
#' \describe{
#' \item{}{\code{mu}: mixture means}
#' \item{}{\code{sigma_sq}: mixture variances}
#' \item{}{\code{class_prob}: mixture weights}
#' }
#' @param tol stopping rule tolerance for change in objective function
#' @param max_steps stopping rule maximum number of steps
#'
#' @return named list with the following components:
#' \describe{
#' \item{}{\code{mu}: mixture means}
#' \item{}{\code{sigma_sq}: mixture variances}
#' \item{}{\code{class_prob}: mixture weights}
#' }
#' @export
#'
#' @examples
#' # Simulate Data
#' n <- 1000
#' K <- 3
#' class_prob <- c(0.5, 0.3, 0.2)
#' mu <- rnorm(3)
#' sigma_sq <- 1/rgamma(3, 1,1)
#' class_label <- sample(1:3, n, replace = T, prob = class_prob)
#' y <- purrr::map_dbl(1:n, ~{
#'   rnorm(1, mu[class_label[.x]], sd = sqrt(sigma_sq[class_label[.x]]))
#' })
#' # Initialize Parameters
#' init <- list(mu = runif(3),
#'              sigma_sq = runif(3),
#'              class_prob = class_prob)
#' # Run EM
#' fit <- gmm_em(y, init)
gmm_r <- function(y, init, tol = 1e-5, max_steps = 1000){
  n <- length(y)
  mu <- init$mu
  sigma_sq <- init$sigma_sq
  class_prob <- init$class_prob
  for(i in 1:max_steps){
    Gamma <- calc_post_prob(y, class_prob, mu, sigma_sq)
    sigma_sq <- update_sigma_sq(y, mu, Gamma)
    mu <- update_mu(y, Gamma)
    class_prob <- colSums(Gamma)/n
  }
  return(list(mu = mu, sigma_sq = sigma_sq, class_prob = class_prob))
}

calc_post_prob <- function(y, prior_prob, mu, sigma_sq){
 P <- purrr::map(1:length(prior_prob), ~{
    prior_prob[.x]*dnorm(y, mu[.x], sd = sqrt(sigma_sq[.x]))
  }) %>%
    do.call(cbind, .)
 P <- sweep(P, 1, rowSums(P), FUN = "/")
}

update_mu <- function(y, Gamma){
  mu <- c((t(Gamma) %*% y)/colSums(Gamma))
}

update_sigma_sq <- function(y, mu, Gamma){
  ss <- outer(y, mu, FUN = function(x,y) (x-y)^2)
  sigma_sq <- c(colSums(ss*Gamma)/colSums(Gamma))
}


#' Calculate density function for Gaussian mixture model
#'
#' @param y vector of points where density should be evaluated
#' @param mu vector of GMM means
#' @param sigma_sq vector of GMM variances
#' @param class_prob vector of GMM mixture probabilities
#'
#' @return
#' @export
#'
#' @examples
#' # Simulate Data
#' n <- 1000
#' K <- 3
#' class_prob <- c(0.5, 0.3, 0.2)
#' mu <- rnorm(3)
#' sigma_sq <- 1/rgamma(3, 1,1)
#' class_label <- sample(1:3, n, replace = T, prob = class_prob)
#' y <- purrr::map_dbl(1:n, ~{
#'   rnorm(1, mu[class_label[.x]], sd = sqrt(sigma_sq[class_label[.x]]))
#' })
#' # Initialize Parameters
#' init <- list(mu = runif(3),
#'              sigma_sq = runif(3),
#'              class_prob = class_prob)
#' # Run EM
#' fit <- gmm_em(y, init)
#'
# Summarize fitted density
#' ygrid <- seq(min(y), max(y), l = n)
#' den <- calc_density(ygrid, fit$mu, fit$sigma_sq, fit$class_prob)
#'
#' # Plot result
#' df <- data.frame(y =y, class_label = as.factor(class_label))
#' ggplot(df) +
#'   geom_density(aes(x = y, color = class_label, group = class_label)) +
#'   geom_density(aes(x = y)) +
#'   geom_line(data = data.frame(x = ygrid, y = den), aes(x = x, y = y),
#'             color = "black", linetype = 2)
calc_density <- function(y, mu, sigma_sq, class_prob){
  purrr::map(1:length(mu), ~{
    dnorm(y, mu[.x], sd = sqrt(sigma_sq[.x]))
  }) %>%
    do.call(cbind, .) %*%
    matrix(class_prob, ncol = 1)
}
