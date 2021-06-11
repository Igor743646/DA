#pragma once
#include <iostream>
#include <math.h>

class Point {
	public:
		double x;
		double y;
		Point(double _x = 0.0, double _y = 0.0);
		Point operator+ (const Point&) const;
		Point operator- (const Point&) const;
		friend Point operator* (const double, const Point&);
		double operator[] (int);
		bool operator== (Point) const;
		bool operator< (Point) const;
		bool operator> (Point) const;
		bool operator!= (Point) const;
		int classify(Point, Point);
		double polarAngle(void);
		double length(void);
		double distance(Point&, Point&);
		friend std::ostream& operator<<(std::ostream&, const Point&);
};

enum {
	LEFT,
	RIGHT,
	BEYOND,
	BEHIND,
	BETWEEN,
	ORIGIN,
	DESTINATION
};