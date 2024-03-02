#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream> // for std::stringstream
#include <utility> // std::pair


// Simple reader for one column
template <typename T>
std::vector<T> read_one_col_file(std::string filename)
{

	// Create a vector of <string, int vector> pairs to store the result
	std::vector<T> result;

	// Read in data from csv
	std::ifstream inFile(filename);

	// Check if the file is open
	if (!inFile.is_open())
	{
		throw std::runtime_error("Could not open file");
	}

	std::string line, colname;
	T value;

	while (std::getline(inFile, line))
	{

		// create stringstream from line
		std::stringstream ss(line);

		// Extract valueue from each row
		while (ss >> value)
		{
			result.push_back(value);
		}
	}
	// Close file
	inFile.close();

	return result;
}

// Write to csv
template <typename T>
void write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<T> > > data_pairs)
{
	// Create output filestream
	std::ofstream outFile(filename);

	// Write column names
	for (int j = 0; j < data_pairs.size(); ++j)
	{
		outFile << data_pairs.at(j).first;
		if (j != data_pairs.size() - 1)
			outFile << ",";
	}
	outFile << "\n";

	// Send data to the stream
	for (int i = 0; i < data_pairs.at(0).second.size(); ++i)
	{
		for (int j = 0; j < data_pairs.size(); ++j)
		{
			outFile << data_pairs.at(j).second.at(i);
			if (j != data_pairs.size() - 1)
				outFile << ",";
		}
		outFile << "\n";
	}

	// Close
	outFile.close();
}


// Read CSV
template <typename T>
std::vector<std::pair<std::string, std::vector<T> > > read_csv(std::string filename)
{
	// Output vector
	std::vector<std::pair<std::string, std::vector<T> > > data;

	// in filestream
	std::ifstream inFile(filename);

	std::string line, colname;
	T value;

	// Read the column names
	if (inFile.good())
	{
		// Extract the first line
		std::getline(inFile, line);

		// Line to string stream
		std::stringstream ss(line);

		// Get column names
		while (std::getline(ss, colname, ','))
		{

			// Initialize vector with <colname, T vector> pairs
			data.push_back({colname, std::vector<T>{}});
		}
	}

	// Read data, line by line
	while (std::getline(inFile, line))
	{
		// Create a stringstream of the current line
		std::stringstream ss(line);

		// Keep track of the current column index
		int colIdx = 0;

		// Extract each integer
		while (ss >> value)
		{
			// Updatewith next column value
			data.at(colIdx).second.push_back(value);

			// Check for comma
			if (ss.peek() == ',')
				ss.ignore();

			colIdx++;
		}
	}

	// Close 
	inFile.close();

	return data;
}

// Print std::vector of cout-able objects
template <typename T>
void print_vector(std::vector<T> v)
{
	for (auto it = v.begin(); it != v.end(); it++)
	{
		std::cout << *it << "\n";
	}
}