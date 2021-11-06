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
	int clusterId, dimension;
	double minDist;

public:
	Point()
	{
		//Default Constructor
		clusterId = -1;
		minDist = DBL_MAX;
		dimension = 0;
	}

	Point(int dimensions)
	{
		//Default Constructor
		this->dimension = dimensions;
		clusterId = -1;
		minDist = DBL_MAX;
		for (int i = 0; i < this->dimension; i++) {
			values.push_back(0.0);
		}
	}

	/* Constructor when reading from file */
	Point(string line)
	{
		clusterId = -1;
		minDist = DBL_MAX;
		dimension = 0;
		stringstream is(line);
		double value;
		while (is >> value) {
			values.push_back(value);
			dimension++;
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


