#include "point.hpp"

Point::Point(double _x, double _y): x(_x), y(_y) {}

Point Point::operator+ (const Point& p) const {
	return Point(x + p.x, y + p.y);
}

Point Point::operator- (const Point& p) const {
	return Point(x - p.x, y - p.y);
}

Point operator* (const double c, const Point& p) {
	return Point(c * p.x, c * p.y);
}

double Point::operator[] (int i) {
	return (i == 0) ? x : y;
}

bool Point::operator== (Point p) const {
	return (x == p.x) and (y == p.y);
}

bool Point::operator< (Point p) const {
	return (x < p.x) or (x == p.x and y < p.y);
}

bool Point::operator> (Point p) const {
	return (x > p.x) or (x == p.x and y > p.y);
}

bool Point::operator!= (Point p) const {
	return !(*this == p);
}

int Point::classify(Point p1, Point p2) {
	Point a = p2 - p1;
	Point b = *this - p1;
	double sa = a.x * b.y - b.x * a.y;
	if (sa > 0.0)
		return LEFT;
	if (sa < 0.0)
		return RIGHT;
	if (a.x * b.x < 0.0 or a.y * b.y < 0.0)
		return BEHIND;
	if (a.length() < b.length())
		return BEYOND;
	if (p1 == *this)
		return ORIGIN;
	if (p2 == *this)
		return DESTINATION;
	return BETWEEN;
}

double Point::polarAngle() {
	if (x == 0 and y == 0) 
		return -1.0;
	if (x == 0)
		return y > 0 ? 90 : 270;
	double theta = atan(y/x);
	theta *= 360 / (2 * 3.1415926);
	if (x > 0.0)
		return y >= 0.0 ? theta : 360 + theta;
	else
		return 180 + theta;
}

double Point::length() {
	return sqrt(x*x + y*y);
}

std::ostream& operator<<(std::ostream& out, const Point& p) {
	return out << "(" << p.x << ", " << p.y << ")";
}
