#ifndef CLUSTER_H
#define CLUSTER_H

#include "Point.h"

class Cluster : public Point
{
private:
	int clusterId;
	vector<double> centroid;
	vector<Point> points;

public:
	Cluster();
	Cluster(int clusterId, Point centroid);
	void addPoint(Point pt);
	bool removePoint(int pointId);
	int getId();
	Point getPoint(int pos);
	int getSize();
	double getCentroidByPos(int pos);
	void setCentroidByPos(int pos, double val);
};

#endif
