#pragma once


#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>
#include "util.hpp"
// #include "dataio.hpp"  // Errors when included


std::vector<std::vector<double> > calc_post_prob(const std::vector<double> &y,
                                                 const std::vector<double> &prior_prob,
                                                 const std::vector<double> &mu,
                                                 const std::vector<double> &sigma_sq)
{

    int K = prior_prob.size();          // Number of classes
    int N = y.size();                   // Number of observations

    // Initialize vector of class probabilities of Z|Y
    std::vector<std::vector<double> > Gamma(N, std::vector<double>(K, 0.0));
    //
    double value, rowsum = 0.0;
    for (int n = 0; n < N; n++)
    {
        rowsum = 0.0;
        for(int k = 0; k < K; k++){
            value = prior_prob[k] * dnorm(y[n], mu[k], std::sqrt(sigma_sq[k]));
            rowsum += value;
            Gamma[n][k] = value;
        }
        // Sweep out rowsums to normalize probabilities
        for(int k = 0; k < K; k++){
            Gamma[n][k] = Gamma[n][k]/ rowsum;
        }
    }
    return Gamma;
}


// Update cluster means
std::vector<double> update_mu(const std::vector<double> y, 
                              const std::vector< std::vector<double> > Gamma){
    int N = y.size();
    int K = Gamma[0].size();
    std::vector<double> mu(K, 0.0);
    std::vector<double> Neff(K, 0.0);

    for (int n = 0; n < N; n++)
    {
        for (int k = 0; k < K; k++)
        {
            mu[k] += Gamma[n][k]*y[n];
            Neff[k] += Gamma[n][k];
        }
    }

    // Divide by effective sample size in each class
    mu = mu / Neff;
    return mu;
}

// Update cluster sds
std::vector<double> update_sigma_sq(const std::vector<double> y, 
                                    const std::vector<double> mu, 
                                    const std::vector<std::vector<double> > Gamma)
{
    int N = y.size();
    int K = Gamma[0].size();
    std::vector<double> colsums(K, 0);
    std::vector<double> innersums(K, 0);

    std::vector<std::vector<double> > ss (N, std::vector<double>(K, 0.0));
    for(int n = 0; n < N; n++)
    {
        for (int k = 0; k < K; k++)
        {
            ss[n][k] = std::pow((y[n] - mu[k]), 2);
        }
    }
    for (int n = 0; n < N; n++)
    {
        ss[n] = ss[n]*Gamma[n];
    }

    for (int n = 0; n < N; n++)
    {
        for (int k = 0; k < K; k++)
        {
            colsums[k] += Gamma[n][k];
            innersums[k] += ss[n][k];
        }
    }
    return innersums/colsums;
}

std::vector<double> update_class_prob(const std::vector<std::vector<double> > Gamma)
{
    int N = Gamma.size();
    int K = Gamma[0].size();
    std::vector<double> class_prob(K, 0);

    for (int n = 0; n < N; n++)
    {
        for (int k = 0; k < K; k++)
        {
            class_prob[k] += Gamma[n][k];
        }
    }
    // Normalize by sample size
    for (int k = 0; k < K; k++)
    {
        class_prob[k] /= N;
    }
    return class_prob;
}

void gmm_em(const std::vector<double> y,
            std::vector<double> &mu,
            std::vector<double> &sigma_sq,
            std::vector<double> & class_prob,
            // const double tol,
            const int max_steps = 1000)
{
            int N = y.size();
            int K = mu.size();
            std::vector<std::vector<double> > Gamma(N, std::vector<double>(K, 0.0));

            // EM loop
            for (int i = 0; i < max_steps; i++)
            {
                Gamma = calc_post_prob(y, class_prob, mu, sigma_sq);
                sigma_sq = update_sigma_sq(y, mu, Gamma);
                mu = update_mu(y, Gamma);
                class_prob = update_class_prob(Gamma);
            }
}
