#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> points(n);
    for (int i = 0; i < n; i++) {
        cin >> points[i].first >> points[i].second;
    }

    // Count vertical pairs (same x, consecutive in y)
    map<int, vector<int>> byX;
    for (auto& p : points) {
        byX[p.first].push_back(p.second);
    }

    long long verticalPairs = 0;
    for (auto& [x, ys] : byX) {
        sort(ys.begin(), ys.end());
        verticalPairs += (long long)ys.size() - 1;
    }

    // Count horizontal pairs (same y, consecutive in x)
    map<int, vector<int>> byY;
    for (auto& p : points) {
        byY[p.second].push_back(p.first);
    }

    long long horizontalPairs = 0;
    for (auto& [y, xs] : byY) {
        sort(xs.begin(), xs.end());
        horizontalPairs += (long long)xs.size() - 1;
    }

    // Count diagonal pairs
    // Sort by x, then by y
    sort(points.begin(), points.end());

    // Group points by x-coordinate
    vector<vector<pair<int, int>>> groups;
    for (int i = 0; i < n; ) {
        int j = i;
        vector<pair<int, int>> group;
        while (j < n && points[j].first == points[i].first) {
            group.push_back(points[j]);
            j++;
        }
        groups.push_back(group);
        i = j;
    }

    long long diagonalPairs = 0;
    set<int> ySet;

    for (auto& group : groups) {
        // For each point in the current group, find predecessor and successor in ySet
        for (auto& p : group) {
            int y = p.second;

            // Find predecessor (largest y' < y)
            auto it = ySet.lower_bound(y);
            if (it != ySet.begin()) {
                diagonalPairs++;
            }

            // Find successor (smallest y' > y)
            it = ySet.upper_bound(y);
            if (it != ySet.end()) {
                diagonalPairs++;
            }
        }

        // Add all y-coordinates of the current group to ySet
        for (auto& p : group) {
            ySet.insert(p.second);
        }
    }

    long long total = verticalPairs + horizontalPairs + diagonalPairs;
    cout << total << endl;

    return 0;
}
