#include "edge.hpp"

Edge::Edge(): org(Point(0, 0)), dest(Point(1, 0)) {}

Edge::Edge(double p1_x, double p1_y, double p2_x, double p2_y) {
	if (p1_x < p2_x) {
		org = Point(p1_x, p1_y);
		dest = Point(p2_x, p2_y);
	} else {
		org = Point(p2_x, p2_y);
		dest = Point(p1_x, p1_y);
	}
}

Edge::Edge(Point& _org, Point& _dest): org(_org), dest(_dest) {
	if (_org < _dest) {
		org = _org;
		dest = _dest;
	} else {
		org = _dest;
		dest = _org;
	}
}

Edge::Edge(Point& _org, Point&& _dest) {
	if (_org < _dest) {
		org = _org;
		dest = _dest;
	} else {
		org = _dest;
		dest = _org;
	}
}

Edge& Edge::rot() {
	Point m = 0.5 * (org + dest);
	Point v = dest - org;
	Point n(v.y, -v.x);
	org = m - 0.5 * n;
	dest = m + 0.5 * n;
	return *this;
}

Edge& Edge::flip() {
	return rot().rot();
}

Point Edge::point(double t) {
	return Point(org + t * (dest - org));
}

double dotProduct(Point& p1, Point&& p2) {
	return (p1.x * p2.x + p1.y * p2.y);
}

int Edge::intersect(Edge& e, double& t) {
	Point a = org;
	Point b = dest;
	Point c = e.org;
	Point d = e.dest;
	Point n = Point((d-c).y, (c-d).x);
	double denom = dotProduct(n, b-a);
	if (denom == 0.0) {
		int aclass = org.classify(e.org, e.dest);
		if (aclass == LEFT or aclass == RIGHT)
			return PARALLEL;
		else
			return COLLINEAR;
	}
	double num = dotProduct(n, a-c);
	t = -num / denom;
	return SKEW;
}

int Edge::intersect(Edge&& e, double& t) {
	Point a = org;
	Point b = dest;
	Point c = e.org;
	Point d = e.dest;
	Point n = Point((d-c).y, (c-d).x);
	double denom = dotProduct(n, b-a);
	if (denom == 0.0) {
		int aclass = org.classify(e.org, e.dest);
		if (aclass == LEFT or aclass == RIGHT)
			return PARALLEL;
		else
			return COLLINEAR;
	}
	double num = dotProduct(n, a-c);
	t = -num / denom;
	return SKEW;
}

int Edge::cross(Edge& e, double& t) {
	double s;
	int crossType = e.intersect(*this, s);
	if (crossType == COLLINEAR or crossType == PARALLEL)
		return crossType;
	if (s < 0.0 or s > 1.0)
		return SKEW_NO_CROSS;
	intersect(e, t);
	if (0.0 <= t and t <= 1.0)
		return SKEW_CROSS;
	else
		return SKEW_NO_CROSS;
}

double Point::distance(Point& p1, Point& p2) {
	Edge ab(p1, p2);
	ab.flip().rot();

	Point n(ab.dest - ab.org);
	n = (1.0 / n.length() * n);

	Edge f(*this, *this + n);
	double t;
	f.intersect(Edge(p1, p2), t);
	return t;
}

bool Edge::isVertical() {
	return org.x == dest.x;
}

double Edge::slope() const {
	if (org.x != dest.x)
		return (dest.y - org.y) / (dest.x - org.x);
	return DBL_MAX;
}

double Edge::y(double x) const {
	return slope() * (x - org.x) + org.y;
}

double Edge::min_y() const {
	return std::min(org.y, dest.y);
}

double Edge::max_y() const {
	return std::max(org.y, dest.y);
}

bool Edge::abovePoint(Point p) const {
	if (org.x <= p.x and p.x <= dest.x and p.y < y(p.x)) {
		return true;
	}
	
	return false;
}

bool Edge::underPoint(Point p) const {
	if (org.x <= p.x and p.x <= dest.x and p.y > y(p.x)) {
		return true;
	}
	
	return false;
}

bool Edge::crossPoint(Point p) const {
	if (org.x <= p.x and p.x <= dest.x and p.y == y(p.x)) {
		return true;
	}
	
	return false;
}

bool Edge::operator==(Edge e) const {
	return (e.org == org and e.dest == dest);
}

bool Edge::operator!=(Edge e) const {
	return (e.org != org or e.dest != dest);
}

std::ofstream& operator<<(std::ofstream& out, const Edge& p) {
	out << p.org.x << " " << p.org.y << " " << p.dest.x << " " << p.dest.y;
	return out;
}

std::ostream& operator<<(std::ostream& out, const Edge& p) {
	out << "[" << p.org << ", " << p.dest << "]";
	return out;
}

std::ifstream& operator>>(std::ifstream& in, Edge& p) {
	in >> p.org.x >> p.org.y >> p.dest.x >> p.dest.y;
	return in;
}