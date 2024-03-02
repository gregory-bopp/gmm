#include "gmm_wrapper.hpp"
#include <R_ext/Rdynload.h>



void R_init_gmm(DllInfo *info) {
  R_RegisterCCallable("gmm", "gmm_ext",  (DL_FUNC) &gmm_wrapper_);
}
