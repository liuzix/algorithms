#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <cassert>

using namespace std;

struct Node {
    vector<Node*> neighbors;
    Node* match = nullptr;
};

bool augmentPath(Node* start, unordered_set<Node*>& visited) {
    if (visited.count(start) > 0) return false;
    visited.insert(start);

    for (Node* v: start->neighbors) {
        if (visited.count(v) > 0) {
            continue;
        } else {
            visited.insert(v);
        }
        if (!v->match) {
            start->match = v;
            v->match = start;
            return true;
        } else {
            if (augmentPath(v->match, visited)) {
                v->match = start;
                start->match = v;
                return true;
            }
        }
    }

    return false;
}

int maxMatch(vector<pair<int, int>>& edges) {
    map<int, Node> lhs, rhs;
    for (auto e: edges) {
        if (lhs.count(e.first) == 0)
            lhs[e.first] = Node();
        if (rhs.count(e.second) == 0)
            rhs[e.second] = Node();
        lhs[e.first].neighbors.push_back(&rhs[e.second]);
        rhs[e.second].neighbors.push_back(&lhs[e.first]);
    }

    unordered_set<Node*> visited;
    bool pathFound;
    do {
        pathFound = false;
        for (auto& p: lhs) {
            if (p.second.match) continue;
            if (augmentPath(&p.second, visited)) {
                pathFound = true;
            }
        }
        visited.clear();
    } while (pathFound);

    int matchCount = 0;
    for (auto p: lhs) {
        if (p.second.match) {
            matchCount ++;
        }
    }

    return matchCount;
}

int bipartite(vector<size_t>& rms, vector<size_t>& cms, vector<vector<int>>& grid) {
    vector<pair<int, int>> edges;
    for (int i = 0; i < rms.size(); i++) {
        for (int j = 0; j < cms.size(); j++) {
            if (grid[rms[i]][cms[j]] != 0) {
                edges.push_back({rms[i], cms[j]});
            }
        }
    }

    auto res = maxMatch(edges);
    return res;
}

int hcount(vector<vector<int>>& grid, vector<int>& rowMax, vector<int>& colMax, int h) {
    assert(h > 1);
    vector<size_t> rms;
    for (int i = 0; i < rowMax.size(); i++) {
        if (rowMax[i] == h)
            rms.push_back(i);
    }

    vector<size_t> cms;
    for (int i = 0; i < colMax.size(); i++) {
        if (colMax[i] == h)
            cms.push_back(i);
    }

    if (rms.size() * cms.size() == 0) {
        return rms.size() + cms.size();
    }

    int bip = bipartite(rms, cms, grid);

    return rms.size() + cms.size() - bip;

}

int main () {
    int r, c;
    cin >> r >> c;
    vector<vector<int>> grid(r, vector<int>(c));
    int initTotal = 0;
    vector<int> rowMax(r);
    vector<int> colMax(c);
    set<int, greater<int>> maxes;
    int sum = 0;
    for (int i = 0; i < r; i++) {
        int rmax = 0;
        for (int j = 0; j < c; j++) {
            int temp;
            cin >> temp;
            grid[i][j] = temp;
            initTotal += temp;
            rmax = max(rmax, temp);
            if (temp != 0) sum ++;
        }
        rowMax[i] = rmax;
        maxes.insert(rmax);
    }

    for (int i = 0; i < c; i++) {
        int cmax = 0;
        for (int j = 0; j < r; j++) {
            cmax = max(cmax, grid[j][i]);
        }
        colMax[i] = cmax;
        maxes.insert(cmax);
    }

    for (int h: maxes) {
        if (h > 1) {
            int hc = hcount(grid, rowMax, colMax, h);
            //cout << "height = " << h << ", count = " <<  hc << endl;
            sum += hc * (h-1);
        }
    }

    cout << initTotal - sum << endl;


    return 0;

}
