#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <csignal>
#include <iomanip>      // std::setprecision

using namespace std;

#define ERR 10e-6

struct vec2 {
    double x, y;
    vec2 (double _x, double _y) : x(_x), y(_y) { }
    bool isInf() const {
        return isinf(x) || isinf(y);
    }
    double length() const {
        return sqrt(x * x + y * y);
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
    bool semiCritical = false;
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

inline bool isBetween(vec2 v1, vec2 v2, vec2 v3) {
    if (v3 == v1 || v3 == v2) return true;
    if (v1 < v3 && v3 < v2) return true;
    if (v2 < v3 && v3 < v1) return true;
    return false;
}

vector<vec2> allIntersection(Point p1, Point p2, Point* start) {
    vector<vec2> ret;
    Point* cur = nullptr;
    while (cur != start) {
        if (!cur) cur = start;
        vec2 ipoint = intersection(p1.v, p2.v, cur->v, cur->next->v);
        if (!ipoint.isInf() && isBetween(cur->v, cur->next->v, ipoint)) {
            ret.push_back(ipoint);
        }
        cur = cur->next;
    }

    if (abs((p1.v - p2.v).length() - 76.157731059) <= ERR) {
        cout << "gwa!" << endl;
    }
    sort(ret.begin(), ret.end());
    return ret;
}

bool checkCross(vec2 e1, vec2 e2, vec2 line) {
    double cross1 = e1.x * line.y - e1.y * line.x;
    double cross2 = e2.x * line.y - e2.y * line.x;
    cout << "cross1 = " << cross1 << " cross2 = " << cross2 << endl;
    return cross1 * cross2 < 0;
}




int main () {
    int n;
    cin >> n;

    vector<Point> points;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        points.emplace_back(x, y);
    }

    unordered_set<Point*, PointHash, PointComp> criticals;
    for (int i = 0; i < n; i++) {
        points[i].prev = &points[(i+n-1) % n];
        points[i].next = &points[(i+n+1) % n];
        cout << "(" << points[i].prev->v.x << "," << points[i].prev->v.y << ")" << endl;
        criticals.insert(&points[i]);
    }

    double maxLen = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            vector<Point*> pIntersection;
            vector<vec2> intersections = allIntersection(points[i], points[j], &points[i]);
            for (int k = 0; k < intersections.size(); k++) {
                Point temp(intersections[k].x, intersections[k].y);
                if (criticals.count(&temp) == 0) {
                    temp.critical = true;
                    pIntersection.push_back(new Point(temp));
                    cout << temp.v.x << " " << temp.v.y << endl;
                } else {
                    vec2 line = points[i].v - points[j].v;
                    Point* pt = *criticals.find(&temp);
                    if (k < intersections.size() - 1 && intersections[k+1] == intersections[k]) {
                        vec2 e1 = pt->next->v - pt->v;
                        vec2 e2 = pt->prev->v - pt->v;

                        //cout << "p = (" << pt->v.x << "," << pt->v.y << ")" << endl;
                        // << "prev = (" << pt->prev->v.x << "," << pt->prev->v.y << ")" << endl;
                        //cout << "next = (" << pt->next->v.x << "," << pt->next->v.y << ")" << endl;
                        if (checkCross(e1, e2, line)) {
                            temp.critical = true;
                            pIntersection.push_back(new Point(temp));
                            cout << temp.v.x << " " << temp.v.y << endl;
                        }
                    } else if (k < intersections.size() - 1) {
                        vec2 e1(0, 0);
                        vec2 e2(0, 0);
                        if (intersections[k+1] == pt->next->v) {
                            e1 = pt->next->next->v - pt->next->v;
                            e2 = pt->prev->v - pt->v;
                        } else if (intersections[k+1] == pt->prev->v){
                            e1 = pt->prev->prev->v - pt->prev->v;
                            e2 = pt->next->v - pt->v;
                        }
                        if (checkCross(e1, e2, line)) {

                        }
                    }
                    k++;
                }
            }
            cout << "----" << endl;

            double len = 0.0;
            bool isInside = false;
            for (int i = 0; i < pIntersection.size(); i++) {
                if (isInside && i > 0) {
                    len += (pIntersection[i]->v - pIntersection[i-1]->v).length();
                    maxLen = max(maxLen, len);
                }
                if (pIntersection[i]->critical) {
                    isInside = !isInside;
                }
                if (!isInside) {
                    len = 0;
                }
            }
        }
    }


    cout.precision(9);
    cout << maxLen;

}
