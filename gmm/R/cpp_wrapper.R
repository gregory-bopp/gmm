#' @useDynLib gmm gmm_wrapper_
#' @export
gmm_cpp <- function(y,
                 mu,
                 sigma_sq,
                 class_prob,
                 max_steps)
  .Call(gmm_wrapper_, y, mu, sigma_sq, class_prob, max_steps)

