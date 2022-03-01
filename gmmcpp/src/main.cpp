#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

#include "dataio.h"
#include "gmm.h"
#include "util.h"

int main(int argc, char *argv[])
{
	////////////////////////////////////////
	// Begin Arg parsing
	////////////////////////////////////////

	// Defaults
	int opt;
	bool verbose = false;
	std::string data_fname;
	std::string init_fname;
	std::string fit_fname = "fit.csv";
	// Optional arguments
	while ((opt = getopt(argc, argv, ":vi:o:")) != -1)
	{
		switch (opt)
		{
		case 'v':
			printf("Verbose mode on");
			verbose = true;
			break;
		case 'i':
			printf("Initial values csv file: %s\n", optarg);
			init_fname = optarg;
			break;
		case 'o':
			printf("Output fit csv file: %s\n", optarg);
			fit_fname = optarg;
			break;
		case ':':
			printf("Option needs a value\n");
			break;
		case '?':
			printf("Unknown option: %c\n", optopt);
			break;
		}
	}
	// Non-option arguments
	for (int i = optind; i < argc; i++)
	{
			printf("Required arg: %s\n", argv[i]);
			data_fname =argv[i];
	}
	////////////////////////////////////////
	// End Arg parsing
	////////////////////////////////////////


	////////////////////////////////////////
	// Load Initial Values Run EM and Save
	////////////////////////////////////////

	// Read in data
	std::vector<std::pair<std::string, std::vector<double> > > data = read_csv<double>(data_fname);
	std::vector<double> y = data[0].second;

	// Read initial values
	std::vector<std::pair<std::string, std::vector<double> > > init = read_csv<double>(init_fname);
	std::string col;
	std::vector<double> mu;
	std::vector<double> sigma_sq;
	std::vector<double> class_prob;
	for (int i = 0; i < init.size(); i++)
	{
		col = init[i].first;
		if(init[i].first == "mu"){
			mu = init[i].second;	
		}
		if (init[i].first == "sigma_sq")
		{
			sigma_sq = init[i].second;
		}
		if (init[i].first == "class_prob")
		{
			class_prob = init[i].second;
		}			
				  
	}

	// Run EM algorithm
	gmm_em(y,
			mu,
			sigma_sq,
			class_prob,
			1000); // Number of steps

	if(verbose){
		// Print out fitted values
		std::cout << "Mu: " << std::endl;
		print_vector(mu);
		std::cout <<  std::endl;
		std::cout << "Sigma^2: "  <<  std::endl;
		print_vector(sigma_sq);
		std::cout <<  std::endl;
		std::cout << "pi: " 	  <<  std::endl;
		print_vector(class_prob);
		std::cout <<  std::endl;
	}
	// Wrap into a vector
	std::vector<std::pair<std::string, std::vector<double> > > fit = {{"mu", mu}, {"sigma_sq", sigma_sq}, {"class_prob", class_prob}};
	// Write to csv
	write_csv(fit_fname, fit);

	return 0;
	}