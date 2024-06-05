#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>
#include <string>
using namespace std;


struct Point {
    double x, y;
    Point(double x_, double y_) : x(x_), y(y_) {

    }
    bool operator==(Point other) const {
        return x == other.x && y == other.y;
    }
};



Point vec(Point a, Point b) { // making vector from a to b
    return {b.x - a.x, b.y - a.y};
}

double operator*(Point a, Point b) { // cross product
    return a.x * b.y - a.y * b.x;
}

double operator%(Point a, Point b) { // scalar product
    return a.x * b.x + a.y * b.y;
}

class Segment {
public:
    Segment(Point a_, Point b_) : a(a_), b(b_) {
    }
    bool PointInSegment(Point x) const {
        if ((x == a) || (x == b)) {
            return false;
        }
        if ((vec(a, b) * vec(a, x)) == 0) {
            if ((vec(a, b) %  vec(a, x)) >= 0) {
                if ((vec(b, a) % vec(b, x)) >= 0) {
                    return true;
                }
            }
        }
        return false;
    }

    bool PointsInDifferentSides(Point x, Point y) const {
        auto first = vec(a, b)  * vec(a, x);
        auto second = vec(a, b) * vec(a, y);
        if (first * second < 0) {
            return true;
        }
        return false;
    }
    bool Intersect(const Segment& other) {
        if (PointsInDifferentSides(other.a, other.b) && other.PointsInDifferentSides(a, b)) {
            return true;
        }
        return false;
    }
private:
    Point a, b;
};


optional<double> convert_to_double(const string& s) {
    double value = stod(s);
    if (errno != 0) {
        return nullopt;
    }
    return value;
}



double square(const vector<Point> &polygon) {
    size_t n = polygon.size();
    double answer = 0;
    // triangulation with point at (0, 0) and calculating square of triangle by cross product
    for (int i = 0; i < n; ++i) {
        answer += polygon[i] * polygon[(i + 1) % n];
    }
    return abs(answer) / 2;
}


bool intersection(const vector<Point> &polygon) {
    // iterating through all pairs of edges
    size_t n = polygon.size();
    for (size_t i = 0; i + 1 < n; ++i) {
        Segment first(polygon[i], polygon[i + 1]);
        for (size_t j = i + 1; j < n; ++j) {
            Segment second(polygon[j], polygon[(j + 1) % n]);
            if (first.Intersect(second)) {
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    if (argc < 9 || argc % 3 != 0) { // should contain at least 3 points
        cout << "Invalid arguments!";
        return 0;
    }

    char pipe[] = "|";
    vector<Point> polygon;

    for (int i = 1; i + 2 <= argc; i += 3) {
        auto get_x = convert_to_double(argv[i]);
        auto get_y = convert_to_double(argv[i + 1]);
        if (!(i + 2 == argc || string(argv[i + 2]) == "|") || get_x == nullopt || get_y == nullopt) {
            cout << "Invalid arguments!";
            return 0;
        }
        polygon.emplace_back(get_x.value(), get_y.value());
    }

    if (intersection(polygon)) {
        cout << "Polygon with self intersection!";
        return 0;
    }

    cout.precision(20);
    cout << square(polygon);
}
