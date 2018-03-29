#include <iostream>

using namespace std;

#define ERR 10e-6

struct vec2 {
    double x, y;
    vec2 (double _x, double _y) : x(_x), y(_y) { }
};

bool operator== (const vec2& v1, const vec2& v2);
bool operator< (const vec2& v1, const vec2& v2);





int main () {

}