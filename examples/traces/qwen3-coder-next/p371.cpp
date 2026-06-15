#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;

    // Special case: no stations and no edges
    if (N == 0 && M == 0) {
        cout << 0 << '\n';
        cout << 0 << '\n';
        return 0;
    }

    int C = M - N + 1;  // number of circle lines

    // Circle lines must be at least 1 (since if no circle lines, then we can't have radial lines, and without circles the only possibility is N=0,M=0)
    if (C < 1) {
        cout << "No solution" << '\n';
        return 0;
    }

    // Compute minimal S (total circle stations with multiplicity) required by the radial constraint
    int x = M + 2 * C - 2;
    int S_min_val = (x + 1) / 2;  // ceiling of x/2
    int S_min = max(3 * C, S_min_val);
    int S_max = min(10 * C, M);

    if (S_min > S_max) {
        cout << "No solution" << '\n';
        return 0;
    }

    // Decompose S_min into C circle lines, each between 3 and 10 stations
    vector<int> circles;
    int rem = S_min - 3 * C;
    for (int i = 0; i < C; i++) {
        int add = min(rem, 7);  // max extra per circle is 7 (since 3+7=10)
        circles.push_back(3 + add);
        rem -= add;
    }

    // Build the circle lines: chain of circles
    vector<vector<int>> lines;
    int next_station = 1;
    for (int i = 0; i < C; i++) {
        vector<int> line;
        if (i == 0) {
            // First circle: no shared station from previous, so just assign consecutive stations
            for (int j = 0; j < circles[i]; j++) {
                line.push_back(next_station + j);
            }
            next_station += circles[i];
        } else {
            // Subsequent circles: start with the last station of the previous circle
            line.push_back(next_station - 1);
            for (int j = 1; j < circles[i]; j++) {
                line.push_back(next_station);
                next_station++;
            }
        }
        lines.push_back(line);
    }

    int D = S_min - (C - 1);   // distinct circle stations
    int R = M - S_min;          // number of radial lines

    // Build radial lines: connect available circle stations (non-transfer) to new radial stations
    vector<pair<int, int>> radials;
    int radial_count = 0;
    int radial_start = D + 1;   // first radial station number

    for (int i = 0; i < C; i++) {
        vector<int>& line = lines[i];
        int start_idx, end_idx;
        if (i == 0) {
            // Only the last station is shared (with circle1), so skip last station
            start_idx = 0;
            end_idx = (int)line.size() - 2;
        } else if (i == C - 1) {
            // Only the first station is shared (with circle C-2), so skip first station
            start_idx = 1;
            end_idx = (int)line.size() - 1;
        } else {
            // Both ends shared, skip first and last
            start_idx = 1;
            end_idx = (int)line.size() - 2;
        }

        for (int j = start_idx; j <= end_idx; j++) {
            if (radial_count >= R) break;
            radials.push_back({line[j], radial_start + radial_count});
            radial_count++;
        }
        if (radial_count >= R) break;
    }

    // Output the solution
    cout << C << '\n';
    for (int i = 0; i < C; i++) {
        cout << lines[i].size();
        for (int station : lines[i]) {
            cout << " " << station;
        }
        cout << '\n';
    }
    cout << R << '\n';
    for (auto& p : radials) {
        cout << p.first << " " << p.second << '\n';
    }

    return 0;
}
