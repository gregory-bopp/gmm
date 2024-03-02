#pragma once


#include <R.h>
#include <Rinternals.h>
#include "gmm.hpp"

extern "C" {
        SEXP gmm_wrapper_(SEXP y_,
                  SEXP mu_,
                  SEXP sigma_sq_,
                  SEXP class_prob_,
                  SEXP max_steps_
        ){


 std::vector<double> y(length(y_));
 std::vector<double> mu(length(mu_));
 std::vector<double> sigma_sq(length(sigma_sq_));
 std::vector<double> class_prob(length(class_prob_));

 double max_steps = asReal(max_steps_);

 // Create std vector objects from passed SEXP
 for (int i = 0; i < length(y_); i++)
   y[i] = REAL_ELT(y_, i);
 for (int i = 0; i < length(mu_); i++)
   mu[i] = REAL_ELT(mu_, i);
 for (int i = 0; i < length(sigma_sq_); i++)
   sigma_sq[i] = REAL_ELT(sigma_sq_, i);
 for (int i = 0; i < length(class_prob_); i++)
    class_prob[i] = REAL_ELT(class_prob_, i);

 // Run GMM EM algorithm
 gmm_em(y, mu, sigma_sq, class_prob, max_steps);

// Cast STD vectors back to SEXP
double *mu_ptr = REAL(mu_);
double *sigma_sq_ptr = REAL(sigma_sq_);
double *class_prob_ptr = REAL(class_prob_);
for (int i = 0; i < length(mu_); i++)
    mu_ptr[i] = mu[i];
for (int i = 0; i < length(sigma_sq_); i++)
    sigma_sq_ptr[i] = sigma_sq[i];
for (int i = 0; i < length(class_prob_); i++)
    class_prob_ptr[i] = class_prob[i];


 //// Alternative creation with NO NAMES
 //// Create a SEXP to return with the fitted values
 // SEXP out = PROTECT(allocVector(VECSXP, 3));           // Create a list with 3 elements

 const char *names[] = {"mu", "sigma_sq", "class_prob", ""};
 SEXP out = PROTECT(Rf_mkNamed(VECSXP, names));
 SET_VECTOR_ELT(out, 0, mu_);                          // Put mu in the first position of list
 SET_VECTOR_ELT(out, 1, sigma_sq_);                    // Put sigma_sq in second
 SET_VECTOR_ELT(out, 2, class_prob_);                  // Put class_prob_ in third

 // Garbage allocation
 UNPROTECT(1);

 return out;
 }
}
