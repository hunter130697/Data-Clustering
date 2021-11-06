#ifndef INTERNALVALIDATION_H
#define INTERNALVALIDATION_H

#include "Point.h"
using namespace std;

double calinskiHarabaz(int k, int n, int dimensions, double SSE, vector<Point>* allPoints, vector<Point>* centroids) {
	double CH = 0.0, trSb = 0.0;
	vector<double> meanValues(dimensions, 0.0);
	Point dataCenter(dimensions);
	vector<int> nPoints(k, 0);

	// Find the mean centroid of all the data points
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < dimensions; j++) {
			meanValues[j] += (*allPoints)[i].getValue(j) / n;
		}
	}

	for (int i = 0; i < dimensions; i++) {
		dataCenter.setValueByPos(i, meanValues[i]);
	}

	// find number of points in each cluster
	for (int i = 0; i < n; i++) {
		int clusterId = (*allPoints)[i].getCluster();
		nPoints[clusterId] += 1;
	}

	// Calculate trace of the between cluster scatter matrix
	for (int j = 0; j < k; j++) {
		double dist = (*centroids)[j].distance(dataCenter);
		trSb += nPoints[j] * dist;
	}

	// Calculate the Calinski-Harabaz index using the following formula:
	// CH = ((n - k) / (k - 1)) * (trSb / SSE)
	CH = ((n - k) / (k - 1)) * (trSb / SSE);

	return CH;
};

double silhouetteWidth(int k, int n, vector<Point>* allPoints) {
	double SW = 0.0, meanIn = 0.0, meanOut = 0.0, silhouette = 0.0;
	int clusterId, innerId, nearestCluster;
	vector<double> dist_vector(k, 0.0);
	vector<int> nPoints(k, 0);

	return SW;
};
#endif // !INTERNALVALIDATION_H

