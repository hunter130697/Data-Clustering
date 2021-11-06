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
#include "Point.h"
#include "K-means.h"
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

		// Normalize values to be between 0 and 1 using the formula:
		// v' = (v - min) / (max - min)
		for (int f = 0; f < totPoints; f++) {
			double normalized = ((*allPoints)[f].getValue(i) - min) / (max - min);
			(*allPoints)[f].setValueByPos(i, normalized);
		}
	}
}

void zScoreNormalize(vector<Point>* allPoints, int totPoints, int dimensions) {
	double mean, variance = 0.0, stdev, sum, normalized;

	for (int i = 0; i < dimensions; i++) {
		// Holds the ith value from each point. Initialized to zero
		vector<double> column(totPoints, 0);

		// Fill column vector with ith value from each point
		for (int j = 0; j < totPoints; j++) {
			column[j] = (*allPoints)[j].getValue(i);
		}

		// Find mean
		sum = accumulate(column.begin(), column.end(), 0.0);
		mean = sum / totPoints;

		// Find variance
		for (int i = 0; i < totPoints; i++) {
			sum = column[i] - mean;
			variance += sum * sum;
		}
		variance /= totPoints;

		// Find standard deviation
		stdev = sqrt(variance);

		// Normalize values using the formula: v' = (v - mean) / stdev
		for (int j = 0; j < totPoints; j++) {
			normalized = ((*allPoints)[j].getValue(i) - mean) / stdev;
			(*allPoints)[j].setValueByPos(i, normalized);
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
	double Threshold;
	double SSE, minSSE;			// Hold return SSE value from kmeans run
	string F ;
	string initType;
	string normalType;
	string line;				// Stores data from input file
	vector<Point> all_points;	// Store all created points
	vector<double> allSSEs;		// Store all SSEs from all runs

	 //Need 8 arguments (except executable filename) to run, else exit
	if (argc != 8) {
		cout << "Error: command-line argument count mismatch.";
		return 1;
	}

	// Initializing Variables
	number_of_cluster = atoi(argv[2]);
	max_of_iteration = atoi(argv[3]);
	Threshold = atof(argv[4]);
	number_of_runs = atoi(argv[5]);
	initType = argv[6];// Initialization method
	normalType = argv[7];// Normalization method

	// Open File for reading
	F = argv[1];
	fstream in_file("D:/data clustering proj/ConsoleApplication1/Data_sets/" + F, ios::in);

	// Test for open
	if (!in_file) {
		cout << "Cannot open " << F << " for input" << endl;
		return 1;
	}

	// Read first line
	getline(in_file, line);
	istringstream(line) >> number_of_points >> Dimension; // First line contains N: number of points and
								   // D: dimensionality of each point

	// Read from file and store points
	while (getline(in_file, line)) {
		Point point(line);
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
	if (all_points.size() < number_of_cluster) {
		cout << "Number of clusters greater than number of points. Try again" << endl;
	}

	// Normalize data
	if (normalType == "minmax") {
		minMaxNormalize(&all_points, number_of_points, Dimension);
	}
	else if (normalType == "zscore") {
		zScoreNormalize(&all_points, number_of_points, Dimension);
	}

	// Update filename for writing
	F.erase(F.end() - 4, F.end());
	F += "Output.txt";

	// Running K-Means clustering
	for (int i = 0; i < number_of_runs; i++) {
		Kmeans kmeans(F, number_of_cluster, max_of_iteration, Threshold, (i + 1), number_of_points, Dimension);
		SSE = kmeans.run(&all_points, initType);
		allSSEs.push_back(SSE);
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
	cout << setprecision(7) << "\nBest Run: " << bestRun << " SSE: " << minSSE << endl;
	out_file << setprecision(7) << "\nBest Run: " << bestRun << " SSE: " << minSSE << endl;
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



