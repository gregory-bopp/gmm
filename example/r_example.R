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
write.csv(data.frame(y), file = file.path(data_dir,"data.csv"), row.names = F, quote = F)

# Initialize Parameters ---------------------------------------------------
init <- list(mu = runif(K),
             sigma_sq = 1/rgamma(K,1,1),
             class_prob = rep(1/K, K))
write.csv(data.frame(init), file = file.path(data_dir, "init.csv"), row.names = F, quote = F)


# Run EM ------------------------------------------------------------------
fit <- gmm_r(y, init, max_steps = 1000)

# Summarize fitted density ------------------------------------------------
ygrid <- seq(min(y), max(y), l = n)
den <- calc_density(ygrid, fit$mu, fit$sigma_sq, fit$class_prob)

# Plot result -------------------------------------------------------------
df <- data.frame(y =y, class_label = as.factor(class_label))
ggplot(df) + 
  geom_density(aes(x = y, fill = "True (overall)"), size = 1) + 
  geom_density(aes(x = y, color = class_label, group = class_label), size = 1) + 
  geom_line(data = data.frame(x = ygrid, y = den), aes(x = x, y = y, 
            color = "Fitted (overall)"), linetype = 2, size = 1) +
  scale_color_manual(name="Class/ Type",
                     values = c("1" = "#02639B",
                                "2" = "#3CAEA3",
                                "3" = "#ED553B",
                                "Fitted (overall)"="black",
                                "True (overall)"="black")) +
  labs(y = "Density", fill = "")




