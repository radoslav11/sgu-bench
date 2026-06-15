#include <iostream>
#include <vector>

/**
 * Problem: 496. L-Shapes
 * Two line segments form an L-shape if they have a 90 degree angle between them 
 * and one endpoint of one segment coincides with one endpoint of the other.
 * 
 * Approach:
 * - Let N be the number of segments. Since N is up to 5000, we can iterate through
 *   all pairs of segments (O(N^2)).
 * - For each pair of segments S_i and S_j, we check if they share an endpoint.
 * - If they share an endpoint, let that endpoint be P. Let the other endpoint 
 *   of segment S_i be Q and the other endpoint of segment S_j be R.
 * - The segments form an L-shape if the dot product of vectors PQ and PR is 0.
 * - We use long long to store coordinates and perform dot product calculations 
 *   to prevent overflow.
 * - Time complexity: O(N^2), Space complexity: O(N).
 */

using namespace std;

// Structure to store segment endpoints
struct Segment {
    long long x1, y1, x2, y2;
};

int main() {
    // Faster input/output for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    // Store the segments in a vector
    vector<Segment> segs(N);
    for (int i = 0; i < N; ++i) {
        cin >> segs[i].x1 >> segs[i].y1 >> segs[i].x2 >> segs[i].y2;
    }

    long long ans = 0;
    // Iterate over all distinct pairs of segments (i, j) where 0 <= i < j < N
    for (int i = 0; i < N; ++i) {
        // Cache endpoints for the current segment i to minimize memory access
        long long x1 = segs[i].x1, y1 = segs[i].y1, x2 = segs[i].x2, y2 = segs[i].y2;
        
        for (int j = i + 1; j < N; ++j) {
            long long x3 = segs[j].x1, y3 = segs[j].y1, x4 = segs[j].x2, y4 = segs[j].y2;

            /*
             * An L-shape occurs if one endpoint matches and the dot product of 
             * vectors from the shared endpoint is zero.
             * There are 4 possible matching endpoint cases:
             * 1. (x1, y1) matches (x3, y3)
             * 2. (x1, y1) matches (x4, y4)
             * 3. (x2, y2) matches (x3, y3)
             * 4. (x2, y2) matches (x4, y4)
             */
            if (x1 == x3 && y1 == y3) {
                // Vectors: (x2-x1, y2-y1) and (x4-x3, y4-y3)
                if ((x2 - x1) * (x4 - x3) + (y2 - y1) * (y4 - y3) == 0) ans++;
            } else if (x1 == x4 && y1 == y4) {
                // Vectors: (x2-x1, y2-y1) and (x3-x4, y3-y4)
                if ((x2 - x1) * (x3 - x4) + (y2 - y1) * (y3 - y4) == 0) ans++;
            } else if (x2 == x3 && y2 == y3) {
                // Vectors: (x1-x2, y1-y2) and (x4-x3, y4-y3)
                if ((x1 - x2) * (x4 - x3) + (y1 - y2) * (y4 - y3) == 0) ans++;
            } else if (x2 == x4 && y2 == y4) {
                // Vectors: (x1-x2, y1-y2) and (x3-x4, y3-y4)
                if ((x1 - x2) * (x3 - x4) + (y1 - y2) * (y3 - y4) == 0) ans++;
            }
        }
    }

    // Output the final count of L-shapes
    cout << ans << endl;

    return 0;
}

