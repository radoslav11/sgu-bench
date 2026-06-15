#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<pair<int, int>> points(N);
    for (int i = 0; i < N; ++i) {
        cin >> points[i].first >> points[i].second;
    }

    // If odd number of points, impossible
    if (N % 2 != 0) {
        cout << 0;
        return 0;
    }

    map<int, vector<int>> by_x; // x -> list of y coordinates
    map<int, vector<int>> by_y; // y -> list of x coordinates

    for (auto& p : points) {
        by_x[p.first].push_back(p.second);
        by_y[p.second].push_back(p.first);
    }

    // Check if each x-group has even number of points
    for (auto& entry : by_x) {
        if ((int)entry.second.size() % 2 != 0) {
            cout << 0;
            return 0;
        }
    }

    // Check if each y-group has even number of points
    for (auto& entry : by_y) {
        if ((int)entry.second.size() % 2 != 0) {
            cout << 0;
            return 0;
        }
    }

    long long horizontal_sum = 0; // Sum of horizontal edge lengths (difference in x for same y)
    long long vertical_sum = 0;   // Sum of vertical edge lengths (difference in y for same x)

    // Process each y-group (horizontal edges)
    for (auto& entry : by_y) {
        vector<int>& xs = entry.second;
        sort(xs.begin(), xs.end());
        for (int i = 0; i < (int)xs.size(); i += 2) {
            horizontal_sum += xs[i + 1] - xs[i];
        }
    }

    // Process each x-group (vertical edges)
    for (auto& entry : by_x) {
        vector<int>& ys = entry.second;
        sort(ys.begin(), ys.end());
        for (int i = 0; i < (int)ys.size(); i += 2) {
            vertical_sum += ys[i + 1] - ys[i];
        }
    }

    cout << horizontal_sum + vertical_sum;

    return 0;
}
