#include "Point.h"
#include <sstream>
#include <iostream>

Point::Point()
{
	//Default Constructor
	clusterId = 0;
	dimensions = 0;
	pointId = 0;
	distance = 0.0;
}

Point::Point(int id, string first_line)
{
	dimensions = 0;
	clusterId = 0;
	pointId = id;
	distance = 0.0;
	stringstream is(first_line);
	double value;
	while (is >> value) {
		values.push_back(value);
		dimensions++;
	}
}

int Point::getDimensions()
{
	return dimensions;
}

int Point::getCluster()
{
	return clusterId;
}

int Point::getId()
{
	return pointId;
}

double Point::getValue(int pos)
{
	return values[pos];
}

double Point::getDistance()
{
	return distance;
}

void Point::setDistance(double distance)
{
	this->distance = distance;
}

void Point::setCluster(int value)
{
	clusterId = value;
}