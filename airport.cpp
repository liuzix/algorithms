#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>

using namespace std;

#define ERR 10e-6

struct vec2 {
    double x, y;
    vec2 (double _x, double _y) : x(_x), y(_y) { }
    bool isInf() const {
        return isinf(x) || isinf(y);
    }
};

bool operator== (const vec2& v1, const vec2& v2) {
    return abs(v1.x - v2.x) < ERR && abs(v1.y - v2.y) < ERR;
}

bool operator< (const vec2& v1, const vec2& v2) {
    if (v1.x != v2.x)
        return v1.x < v2.x;
    else
        return v1.y < v2.y;
}

vec2 operator- (const vec2& a, const vec2& b) {
    return vec2(a.x - b.x, a.y - b.y);
}

struct Point {
    vec2 v;
    Point* next;
    Point* prev;
    bool critical = false;
    Point (double x, double y) : v(x, y) {}
};

struct PointHash {
    size_t operator()(const Point* p) const {
        return (hash<int>()((int)p->v.x) ^ hash<int>()((int)p->v.y));
    }
};

struct PointComp {
    size_t operator()(Point* const&  p1, Point* const& p2) const {
        return p1->v == p2->v;
    }
};

vec2 intersection(vec2 p1, vec2 p2, vec2 p3, vec2 p4) {
    double x1 = p1.x;
    double y1 = p1.y;
    double x2 = p2.x;
    double y2 = p2.y;
    double x3 = p3.x;
    double y3 = p3.y;
    double x4 = p4.x;
    double y4 = p4.y;

    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    double xnom = (x1 * y2 - y1 * x2) * (x3 - x4)
                - (x1 - x2) * (x3 * y4 - y3 * x4);

    double ynom = (x1 * y2 - y1 * x2) * (y3 - y4)
                - (y1 - y2) * (x3 * y4 - y3 * x4);

    return vec2(xnom/denom, ynom/denom);
}

vector<Point> allIntersection(Point p1, Point p2,
                              unordered_set<Point*, PointHash, PointComp>& criticals,
                              Point* start) {
    vector<vec2> ret;
    Point* cur = start;
    while (cur != start) {
        vec2 ipoint = intersection(p1.v, p2.v, cur->v, cur->next->v);
        if (ipoint.isInf()) {
            cur = cur->next;
        } else {
            ret.push_back(ipoint);
            cur = cur->next;
        }
    }
}



int main () {
    int n;
    cin >> n;

    vector<Point> points;
    for (int i = 0; i < n; i++) {
        int x, y;
        points.emplace_back(x, y);
    }

    for (int i = 0; i < n; i++) {
        points[i].prev = &points[(i-1) % n];
        points[i].next = &points[(i+1) % n];
    }

}
