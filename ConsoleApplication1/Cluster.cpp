#include "Cluster.h"

Cluster::Cluster()
{
	// Default Constructor
	clusterId = 0;
}

Cluster::Cluster(int clusterId, Point centroid)
{
	this->clusterId = clusterId;
	for (int i = 0; i < centroid.getDimensions(); i++) {
		this->centroid.push_back(centroid.getValue(i));
	}
	this->addPoint(centroid);
}

void Cluster::addPoint(Point pt)
{
	pt.setCluster(this->clusterId);
	points.push_back(pt);
}

bool Cluster::removePoint(int pointId)
{
	int size = points.size();

	for (int i = 0; i < size; i++) {
		if (points[i].getId() == pointId) {
			points.erase(points.begin() + 1);
			return true;
		}
	}
	return false;
}

int Cluster::getId()
{
	return clusterId;
}

Point Cluster::getPoint(int pos)
{
	return points[pos];
}

int Cluster::getSize()
{
	return points.size();
}

double Cluster::getCentroidByPos(int pos)
{
	return centroid[pos];
}

void Cluster::setCentroidByPos(int pos, double val)
{
	this->centroid[pos] = val;
}