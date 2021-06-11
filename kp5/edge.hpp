#pragma once
#include "point.hpp"
#include <cfloat>
#include <fstream>

class Edge {
	public:
		Point org;
		Point dest;
		Edge(void);
		Edge(double p1_x, double p1_y, double p2_x, double p2_y);
		Edge(Point& _org, Point& _dest);
		Edge(Point& _org, Point&& _dest);
		Edge& rot(void);
		Edge& flip(void);
		Point point(double);
		int intersect(Edge&, double&);
		int intersect(Edge&&, double&);
		int cross(Edge&, double&);
		bool isVertical(void);
		double slope(void) const;
		double y(double) const;
		bool abovePoint(Point) const;
		bool underPoint(Point) const;
		bool crossPoint(Point) const;
		bool operator==(Edge) const;
		bool operator!=(Edge) const;
		double min_y() const;
		double max_y() const;
		friend std::ofstream& operator<<(std::ofstream&, const Edge&);
		friend std::ostream& operator<<(std::ostream&, const Edge&);
		friend std::ifstream& operator>>(std::ifstream&, Edge&);
};

enum {
	COLLINEAR,
	PARALLEL,
	SKEW,
	SKEW_CROSS,
	SKEW_NO_CROSS
};