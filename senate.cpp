#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void solve(int caseNum) {
    cout << "Case #" << caseNum << ": ";
    int n;
    cin >> n;
    auto cmp = [](pair<int, int>& p1, pair<int, int>& p2) {
        return p1.second < p2.second;
    };

    vector<pair<int, int>> numbers;
    for (int i = 0; i < n; i++) {
        int k;
        cin >> k;
        numbers.emplace_back(i, k);
    }

    make_heap(numbers.begin(), numbers.end(), cmp);

    while (!numbers.empty()) {
        if (numbers.size() > 2 || numbers.size() == 1) {
            pop_heap(numbers.begin(), numbers.end(), cmp);
            auto p = numbers.back();
            numbers.pop_back();
            int party = p.first;
            p.second--;
            char c = party + 'A';
            cout << c << " ";
            if (p.second != 0) {
                numbers.push_back(p);
                push_heap(numbers.begin(), numbers.end(), cmp);
            }
        } else {
            int p1 = numbers[0].first;
            int p2 = numbers[1].first;
            numbers[0].second--;
            numbers[1].second--;

            char c1 = p1 + 'A';
            char c2 = p2 + 'A';
            cout << c1 << c2 << " ";

            if (numbers[1].second == 0)
                numbers.erase(numbers.begin() + 1, numbers.begin() + 2);
            if (numbers[0].second == 0)
                numbers.erase(numbers.begin(), numbers.begin() + 1);
        }
    }

    cout << endl;
}

int main () {
    int t;
    cin >> t;
    for (int i = 1; i <= t; i++) {
        solve(i);
    }
}
