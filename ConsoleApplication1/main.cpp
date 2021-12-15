// Anh Dang
// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Reference: https://google.github.io/styleguide/cppguide.html

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <numeric>
#include <cmath>
#include <sstream>
#include <random>
#include "Point.h"
#include "K-means.h"
#include "ExternalValidation.h"
using namespace std;


void minMaxNormalize(vector<Point>* allPoints, int totPoints, int dimensions) {
	double min, max;

	for (int i = 0; i < dimensions; i++) {
		// Holds the ith value from each point. Initialized to zero
		vector<double> column(totPoints, 0);

		// Fill column vector with ith value from each point
		for (int j = 0; j < totPoints; j++) {
			column[j] = (*allPoints)[j].getValue(i);
		}

		// Find min and max value
		auto result = minmax_element(column.begin(), column.end());
		min = *result.first;
		max = *result.second;

		if (max - min == 0) {
			max = 1;
			min = 0;
		}

		// Normalize values to be between 0 and 1 using the formula:
		// v' = (v - min) / (max - min)
		for (int f = 0; f < totPoints; f++) {
			double normalized = ((*allPoints)[f].getValue(i) - min) / (max - min);
			(*allPoints)[f].setValueByPos(i, normalized);
		}
	}
}

int main(int argc, char** argv) {
	int number_of_cluster;
	int max_of_iteration;
	int number_of_runs;
	int number_of_points; 
	int Dimension; 
	int bestRun;
	int number_of_attributes;
	double Threshold, rand, jaccard, fowlkes;
	double SSE, minSSE;			// Hold return SSE value from kmeans run
	string F ;
	string initType;
	string normalType;
	string line;				// Stores data from input file
	vector<Point> all_points;	// Store all created points
	vector<double> allSSEs, allRand, allJaccard, allFowlkes;		// Store all SSEs from all runs

	 //Need 5 arguments (except executable filename) to run, else exit
	if (argc != 5) {
		cout << "Error: command-line argument count mismatch.";
		return 1;
	}

	// Initializing Variables
	max_of_iteration = atoi(argv[2]);
	Threshold = atof(argv[3]);
	number_of_runs = atoi(argv[4]);

	// Open File for reading
	F = argv[1];
	fstream in_file("./phase4_data_sets/" + F, ios::in);

	// Test for open
	if (!in_file) {
		cout << "Cannot open " << F << " for input" << endl;
		return 1;
	}

	// Read first line
	getline(in_file, line);
	istringstream(line) >> number_of_points >> number_of_attributes >> number_of_cluster; 
								   
	Dimension = number_of_attributes - 1;

	// Read from file and store points
	while (getline(in_file, line)) {
		Point point(line, Dimension);
		all_points.push_back(point);
	}
	in_file.close();

	// Check all points imported
	if (all_points.size() != number_of_points) {
		cout << "\nError: incorrect number of data points imported." << endl << endl;
	}
	else
	{
		cout << "\nData Imported sucessfully" << endl << endl;
	}

	// Return if number of clusters > number of points
	if ((int)all_points.size() < number_of_cluster) {
		cout << "Number of clusters greater than number of points. Try again" << endl;
	}

	// Normalize data
	minMaxNormalize(&all_points, number_of_points, Dimension);

	// Update filename for writing
	F.erase(F.end() - 4, F.end());
	F += "Output.txt";

	// Running K-Means clustering
	for (int i = 0; i < number_of_runs; i++) {
		random_device random;
		mt19937 rng(random());
		uniform_int_distribution<int> uni(0, number_of_cluster - 1);

		for (int j = 0; j < number_of_points; j++) {
			auto random_integer = uni(rng);
			all_points[j].setCluster(random_integer);
		}


		Kmeans kmeans(number_of_cluster, max_of_iteration, Threshold, (i + 1), number_of_points, Dimension);
		SSE = kmeans.run(&all_points);
		allSSEs.push_back(SSE);

		ExternalValidation external(number_of_cluster, number_of_points);
		external.calculateTable(&all_points);
		external.calculateMeasures();
		rand = external.randIndex();
		jaccard = external.jaccardCoefficient();
		fowlkes = external.fowlkesMallows();

		allRand.push_back(rand);
		allJaccard.push_back(jaccard);
		allFowlkes.push_back(fowlkes);
	}

	// Find Best Run
	bestRun = min_element(allSSEs.begin(), allSSEs.end()) - allSSEs.begin() + 1;
	minSSE = *min_element(allSSEs.begin(), allSSEs.end());

	
	// Output best Run
	fstream out_file(F, ios::app);
	if (!out_file) {
		cout << "Error: Cannot open " << F << " for writing" << endl;
		return 1;
	}

	// Find and print lowest SSE
	bestRun = min_element(allSSEs.begin(), allSSEs.end()) - allSSEs.begin() + 1;
	minSSE = *min_element(allSSEs.begin(), allSSEs.end());

	cout << setprecision(7) << "Lowest SSE at Run " << bestRun << ": " << minSSE << endl;
	out_file << setprecision(7) << "Lowest SSE at Run " << bestRun << ": " << minSSE << endl;

	// Find and print highest Rand Index
	bestRun = max_element(allRand.begin(), allRand.end()) - allRand.begin() + 1;
	rand = *max_element(allRand.begin(), allRand.end());

	cout << setprecision(7) << "Best Rand Index at Run " << bestRun << ": " << rand << endl;
	out_file << setprecision(7) << "Best Rand Index at Run " << bestRun << ": " << rand << endl;

	// Find and print highest Jaccard Coefficient
	bestRun = max_element(allJaccard.begin(), allJaccard.end()) - allJaccard.begin() + 1;
	jaccard = *max_element(allJaccard.begin(), allJaccard.end());

	cout << setprecision(7) << "Best Jaccard Coefficient at Run " << bestRun << ": " << jaccard << endl;
	out_file << setprecision(7) << "Best Jaccard Coefficient at Run " << bestRun << ": " << jaccard << endl;

	// Find and print highest Fowlkes-Mallows Index
	bestRun = max_element(allFowlkes.begin(), allFowlkes.end()) - allFowlkes.begin() + 1;
	fowlkes = *max_element(allFowlkes.begin(), allFowlkes.end());

	cout << setprecision(7) << "Best Fowlkes-Mallows Index at Run " << bestRun << ": " << fowlkes << endl;
	out_file << setprecision(7) << "Best Fowlkes-Mallows Index at Run " << bestRun << ": " << fowlkes << endl;

	out_file.close();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file



