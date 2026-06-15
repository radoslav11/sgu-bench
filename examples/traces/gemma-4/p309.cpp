#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: 309. Real Fun
 * Objective: Find the minimum side length d of three squares that can cover all given n points.
 * Constraints: n up to 20,000. Coordinates up to 10^9. Time limit 0.75s.
 * Approach: Binary search on the answer d. For a fixed d, use a corner-based approach to 
 * check if three squares can cover the points.
 */

struct Point {
    long long x, y;
};

// Check if all points in the given indices can be covered by a single d x d square.
bool can_cover_1_square(long long d, const vector<Point>& points, const vector<int>& indices) {
    if (indices.empty()) return true;
    long long xmin = 2000000007LL, xmax = -2000000007LL, ymin = 2000000007LL, ymax = -2000000007LL;
    for (int idx : indices) {
        if (points[idx].x < xmin) xmin = points[idx].x;
        if (points[idx].x > xmax) xmax = points[idx].x;
        if (points[idx].y < ymin) ymin = points[idx].y;
        if (points[idx].y > ymax) ymax = points[idx].y;
    }
    return (xmax - xmin <= d && ymax - ymin <= d);
}

// Check if all points in the given indices can be covered by two d x d squares.
bool can_cover_2_squares(long long d, const vector<Point>& points, const vector<int>& indices) {
    if (indices.empty()) return true;
    long long xmin = 2000000007LL, xmax = -2000000007LL, ymin = 2000000007LL, ymax = -2000000007LL;
    for (int idx : indices) {
        if (points[idx].x < xmin) xmin = points[idx].x;
        if (points[idx].x > xmax) xmax = points[idx].x;
        if (points[idx].y < ymin) ymin = points[idx].y;
        if (points[idx].y > ymax) ymax = points[idx].y;
    }
    // If the entire bounding box can be covered by one square, return true.
    if (xmax - xmin <= d && ymax - ymin <= d) return true;

    // Try placing the first square at one of the four corners of the current bounding box.
    long long sx[4] = {xmin, xmin, xmax - d, xmax - d};
    long long sy[4] = {ymin, ymax - d, ymin, ymax - d};

    for (int i = 0; i < 4; ++i) {
        vector<int> next_indices;
        next_indices.reserve(indices.size());
        long long s_xmin = sx[i], s_xmax = sx[i] + d;
        long long s_ymin = sy[i], s_ymax = sy[i] + d;
        for (int idx : indices) {
            if (points[idx].x < s_xmin || points[idx].x > s_xmax ||
                points[idx].y < s_ymin || points[idx].y > s_ymax) {
                next_indices.push_back(idx);
            }
        }
        // Check if the remaining points can be covered by the second square.
        if (can_cover_1_square(d, points, next_indices)) return true;
    }
    return false;
}

// Check if all points in the given indices can be covered by three d x d squares.
bool check(long long d, const vector<Point>& points, const vector<int>& indices) {
    if (indices.empty()) return true;
    long long xmin = 2000000007LL, xmax = -2000000007LL, ymin = 2000000007LL, ymax = -2000000007LL;
    for (int idx : indices) {
        if (points[idx].x < xmin) xmin = points[idx].x;
        if (points[idx].x > xmax) xmax = points[idx].x;
        if (points[idx].y < ymin) ymin = points[idx].y;
        if (points[idx].y > ymax) ymax = points[idx].y;
    }
    if (xmax - xmin <= d && ymax - ymin <= d) return true;

    // Try placing the first square at one of the four corners of the current bounding box.
    long long sx[4] = {xmin, xmin, xmax - d, xmax - d};
    long long sy[4] = {ymin, ymax - d, ymin, ymax - d};

    for (int i = 0; i < 4; ++i) {
        vector<int> next_indices;
        next_indices.reserve(indices.size());
        long long s_xmin = sx[i], s_xmax = sx[i] + d;
        long long s_ymin = sy[i], s_ymax = sy[i] + d;
        for (int idx : indices) {
            if (points[idx].x < s_xmin || points[idx].x > s_xmax ||
                points[idx].y < s_ymin || points[idx].y > s_ymax) {
                next_indices.push_back(idx);
            }
        }
        // Check if the remaining points can be covered by the remaining two squares.
        if (can_cover_2_squares(d, points, next_indices)) return true;
    }
    return false;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;
    vector<Point> points(n);
    for (int i = 0; i < n; ++i) {
        cin >> points[i].x >> points[i].y;
    }

    // Initial set of indices representing all points.
    vector<int> indices(n);
    for (int i = 0; i < n; ++i) indices[i] = i;

    // Binary search for the minimum side length d.
    long long low = 0, high = 2000000000LL, ans = 2000000000LL;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (check(mid, points, indices)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    cout << ans << endl;

    return 0;
}

