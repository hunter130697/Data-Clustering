#ifndef POINT_H
#define POINT_H

#include <vector>
#include <string>
#include <sstream>
#include <cfloat>

using namespace std;

class Point
{
private:
	vector<double> values;
	int clusterId, dimension, partitionID;
	double minDist;

public:
	Point()
	{
		//Default Constructor
		clusterId = -1;
		minDist = DBL_MAX;
		dimension = 0;
		partitionID = -1;
	}

	Point(int dimensions)
	{
		//Default Constructor
		this->dimension = dimensions;
		clusterId = -1;
		minDist = DBL_MAX;
		partitionID = -1;
		for (int i = 0; i < this->dimension; i++) {
			values.push_back(0.0);
		}
	}

	/* Constructor when reading from file */
	Point(string line, int D)
	{
		clusterId = -1;
		minDist = DBL_MAX;
		partitionID = -1;
		dimension = 0;
		stringstream is(line);
		double value;
		while (is >> value) {
			if (dimension < D) {
				values.push_back(value);
				dimension++;
			}
			else {
				partitionID = value;
			}
		}
	}

	int getCluster()
	{
		return clusterId;
	}

	double getValue(int pos)
	{
		return values[pos];
	}

	double getMinDist()
	{
		return minDist;
	}

	double getPartition()
	{
		return partitionID;
	}

	void setMinDist(double distance)
	{
		this->minDist = distance;
	}

	void setCluster(int value)
	{
		clusterId = value;
	}

	void setValueByPos(int pos, double val)
	{
		values[pos] = val;
	}

	double distance(Point& pt)
	{
		double sum = 0.0, delta = 0.0;
		for (int i = 0; i < values.size(); i++) {
			delta = pt.getValue(i) - this->getValue(i);
			sum += delta * delta;
		}

		return sum;
	}

};

#endif


