#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem Summary:
 * We are given n hardened segments, each either horizontal or vertical.
 * No two horizontal segments overlap or touch.
 * No two vertical segments overlap or touch.
 * We need to find the number of rectangles that can be formed such that
 * all four sides of the rectangle are part of the given segments.
 * 
 * Rectangle properties:
 * 1. Positive area.
 * 2. Sides parallel to the axes.
 * 3. The perimeter is completely covered by the segments.
 * 
 * Key observation:
 * Since horizontal segments do not touch, each horizontal side of the rectangle 
 * must be completely contained within a single horizontal segment.
 * Similarly, each vertical side of the rectangle must be completely contained
 * within a single vertical segment.
 * 
 * Algorithm:
 * 1. Categorize the segments into horizontal (H) and vertical (V).
 * 2. Sort the vertical segments by their x-coordinate.
 * 3. For every pair of vertical segments (v_i, v_j) with different x-coordinates:
 *    a. Find their common y-range: [y_low, y_high], where
 *       y_low = max(v_i.y1, v_j.y1) and y_high = min(v_i.y2, v_j.y2).
 *    b. If y_low < y_high:
 *       i. Identify all horizontal segments h that cover the interval [v_i.x, v_j.x]
 *          and have a y-coordinate in the range [y_low, y_high].
 *       ii. Let 'k' be the number of such segments.
 *       iii. Each pair of such horizontal segments forms a valid rectangle.
 *            The number of such pairs is k * (k - 1) / 2.
 * 4. The complexity is approximately O(N_V^2 * N_H), which is at most 
 *    around 16 million operations for n = 600, well within the time limit.
 */

struct SegH {
    long long y, x1, x2;
};

struct SegV {
    long long x, y1, y2;
};

int main() {
    // Improve I/O performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<SegH> H;
    vector<SegV> V;

    for (int i = 0; i < n; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        if (x1 == x2) {
            // Vertical segment
            V.push_back({x1, min(y1, y2), max(y1, y2)});
        } else {
            // Horizontal segment
            H.push_back({y1, min(x1, x2), max(x1, x2)});
        }
    }

    // Sort vertical segments by x-coordinate to simplify traversal
    sort(V.begin(), V.end(), [](const SegV& a, const SegV& b) {
        return a.x < b.x;
    });

    long long total_rectangles = 0;
    int num_v = V.size();
    int num_h = H.size();

    for (int i = 0; i < num_v; ++i) {
        for (int j = i + 1; j < num_v; ++j) {
            // Rectangle must have a positive width
            if (V[i].x == V[j].x) continue;

            long long x_low = V[i].x;
            long long x_high = V[j].x;

            // Calculate the intersection of the y-ranges of the two vertical segments
            long long y_low = max(V[i].y1, V[j].y1);
            long long y_high = min(V[i].y2, V[j].y2);

            // If there is a common y-range, look for suitable horizontal segments
            if (y_low < y_high) {
                int count = 0;
                for (int k = 0; k < num_h; ++k) {
                    // Segment must cover the x-interval [x_low, x_high]
                    // and its y-coordinate must lie within the [y_low, y_high] range.
                    if (H[k].y >= y_low && H[k].y <= y_high &&
                        H[k].x1 <= x_low && H[k].x2 >= x_high) {
                        count++;
                    }
                }
                // Choose any two of these horizontal segments to form a rectangle
                total_rectangles += (long long)count * (count - 1) / 2;
            }
        }
    }

    cout << total_rectangles << endl;

    return 0;
}

