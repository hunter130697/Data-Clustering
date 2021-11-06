#pragma once
#ifndef KMEANS_H
#define KMEANS_H

#include "Point.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cfloat>
#include <iomanip>

using namespace std;

class Kmeans : public Point
{
private:
	int clusters, Iteration, Dimension, numRun, totPoints;
	double threshold;
	string filename;
	vector<Point> centroids;

public:
	Kmeans()
	{
		// Default Constructor
		filename = "";
		clusters = 0;
		Iteration = 0;
		threshold = 0.0;
		numRun = 0;
		totPoints = 0;
		Dimension = 0;

	}

	Kmeans(string fn, int Clusters, int iterations, double threshold, int numRun, int N, int dimensions)
	{
		this->filename = fn;
		this->clusters = Clusters;
		this->Iteration = iterations;
		this->threshold = threshold;
		this->numRun = numRun;
		this->totPoints = N;
		this->Dimension = dimensions;

	}

	//find the closest distance between centroids
	void calculateCentroids(vector<Point>* allPoints) {
		vector<int> nPoints(clusters, 0);
		vector<Point> resetCentroids;

		// Create point vector of empty points
		for (int i = 0; i < clusters; i++) {
			Point pt(Dimension);
			resetCentroids.push_back(pt);
		}

		// Swap empty point vector with centroid vector to reset values
		centroids.swap(resetCentroids);

		// Sum up values for each coordinate and save in centroids vector
		for (int i = 0; i < totPoints; i++) {
			int clusterId = (*allPoints)[i].getCluster();
			nPoints[clusterId] += 1;
			for (int j = 0; j < Dimension; j++) {
				double sum = centroids[clusterId].getValue(j) + (*allPoints)[i].getValue(j);
				centroids[clusterId].setValueByPos(j, sum);
			}

			(*allPoints)[i].setMinDist(DBL_MAX); // reset distance
		}

		// Divide values of centroid coordinates by # of points in cluster to obtain mean
		for (int i = 0; i < clusters; i++) {
			for (int j = 0; j < Dimension; j++) {
				double mean = centroids[i].getValue(j) / nPoints[i];
				centroids[i].setValueByPos(j, mean);
			}
		}
	}

	/*Randomly selects K points from initial clusters*/
	void randSelection(vector<Point>* allPoints) {
		srand(time(0));

		for (int i = 0; i < clusters; i++) {
			int index = rand() % totPoints;
			centroids.push_back((*allPoints)[index]);
		}

	}

	/* Assigns each point to a cluster selected uniformly at random */
	void randPartition(vector<Point>* allPoints) {
		srand(time(0));

		for (int i = 0; i < totPoints; i++) {
			int assignment = rand() % clusters;
			(*allPoints)[i].setCluster(assignment);
		}

		/// Calculate centroids of randomly partitioned clusters
		calculateCentroids(allPoints);
	}

	/*This method chooses the first center arbitrarily from the data points and the remaining (K − 1) centers are chosen
	successively*/
	void maximin(vector<Point>* allPoints) {
		vector<double> meanValues(Dimension, 0.0);
		Point pt(Dimension);
		int max_dist_index = 0;
		double dist, max_dist;

		// Initialize first centroid as average of all point values
		for (int i = 0; i < totPoints; i++) {
			for (int j = 0; j < Dimension; j++) {
				meanValues[j] += (*allPoints)[i].getValue(j) / totPoints;
			}
		}

		for (int i = 0; i < Dimension; i++) {
			pt.setValueByPos(i, meanValues[i]);
		}

		centroids.push_back(pt);

		// Choose the remaining centers using maximin
		for (int i = 1; i < clusters; i++) {
			max_dist = DBL_MIN;
			for (int j = 0; j < totPoints; j++) {
				// Compute points distance to previously chosen centroid
				dist = centroids[i - 1].distance((*allPoints)[j]);

				if (dist < (*allPoints)[j].getMinDist()) {
					// Update nearest-centroid-distance for this point
					(*allPoints)[j].setMinDist(dist);
				}
				if (max_dist < (*allPoints)[j].getMinDist()) {
					// Update the maximum nearest-centroid-distance so far
					max_dist = (*allPoints)[j].getMinDist();
					max_dist_index = j;
				}
			}

			// Point with maximum distance to its nearest center is chosen as a centroid
			centroids.push_back((*allPoints)[max_dist_index]);
		}

	}

	double run(vector<Point>* allPoints, string initType = "randSelection")
	{

		// Initializing Clusters
		if (initType == "randSelection") {
			randSelection(allPoints);
		}
		else if (initType == "randPartition") {
			randPartition(allPoints);
		}
		else if (initType == "maximin") {
			maximin(allPoints);
		}


		cout << "clusters initialized = " << centroids.size() << endl << endl;

		cout << "Running K-Means Clustering... " << endl;

		int currentIter = 1;
		double prevSSE = INT_MAX;
		double currentSSE = 0.0;
		bool belowThreshold = false;

		// Open file for writing
		fstream out_file(filename, ios::out | ios::app);

		//Test for opening
		if (!out_file) {
			cout << "Cannot open " << filename << "Output.txt for writing" << endl;
		}
		cout << "\nRun " << numRun << endl;
		out_file << "\nRun " << numRun << endl;
		cout << "-----" << endl;
		out_file << "-----" << endl;

		do {
			cout << "Iteration " << currentIter << ": ";
			out_file << "Iteration " << currentIter << ": ";

			// Add all points to their nearest cluster
			for (vector<Point>::iterator it = allPoints->begin();
				it != allPoints->end(); ++it) {
				Point& p = *it;

				for (vector<Point>::iterator c = begin(centroids);
					c != end(centroids); ++c) {
					int clusterId = c - begin(centroids);
					double dist = c->distance(p);
					if (dist <= p.getMinDist()) {
						p.setMinDist(dist);
						p.setCluster(clusterId);
					}
				}

				// Add min distance to SSE
				currentSSE += it->getMinDist();
			}

			// Print total SSE
			cout << "SSE = " << std::setprecision(7) << currentSSE << endl;
			out_file << "SSE = " << std::setprecision(7) << currentSSE << endl;

			// Check for threshold
			if ((abs(currentSSE - prevSSE) / prevSSE) < threshold) {
				belowThreshold = true;
			}

			prevSSE = currentSSE; // Save SSE for next iteration
			currentSSE = 0.0;	  // Reset currentSSE

			// Recalculate the centroid of each cluster
			calculateCentroids(allPoints);

			currentIter++;
		} while (currentIter <= Iteration && !belowThreshold);

		out_file.close();
		return prevSSE;
	}

};

#endif

