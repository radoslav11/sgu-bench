#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

/**
 * Problem: 124. Broken line
 * Determine whether a point (X0, Y0) is inside, outside, or on the border of a closed 
 * broken line with axis-parallel sides.
 * 
 * Approach:
 * 1. Use a ray casting algorithm to check if a point is inside a polygon.
 * 2. A horizontal ray (positive X direction) is cast from (X0, Y0).
 * 3. First, check if (X0, Y0) lies on any of the segments (BORDER).
 * 4. Count intersections of the ray with vertical segments. 
 *    A vertical segment (x_s, y_min) to (x_s, y_max) intersects the ray if:
 *    - x_s > X0
 *    - y_min <= Y0 < y_max (half-open interval to handle vertices correctly)
 * 5. If the count of intersections is odd, the point is INSIDE; else, OUTSIDE.
 */

struct Segment {
    int x1, y1, x2, y2;
};

int main() {
    int K;
    // Read the number of segments K
    if (scanf("%d", &K) != 1) return 0;
    
    vector<Segment> segments(K);
    for (int i = 0; i < K; ++i) {
        // Read the endpoints of each segment
        if (scanf("%d %d %d %d", &segments[i].x1, &segments[i].y1, &segments[i].x2, &segments[i].y2) != 4) break;
    }
    
    int X0, Y0;
    // Read the target point (X0, Y0)
    if (scanf("%d %d", &X0, &Y0) != 2) return 0;

    bool on_border = false;
    for (int i = 0; i < K; ++i) {
        int x_min = min(segments[i].x1, segments[i].x2);
        int x_max = max(segments[i].x1, segments[i].x2);
        int y_min = min(segments[i].y1, segments[i].y2);
        int y_max = max(segments[i].y1, segments[i].y2);

        // Since sides are parallel to coordinate axes, each segment is either vertical or horizontal.
        if (segments[i].x1 == segments[i].x2) { // Vertical segment
            if (segments[i].x1 == X0 && Y0 >= y_min && Y0 <= y_max) {
                on_border = true;
                break;
            }
        } else if (segments[i].y1 == segments[i].y2) { // Horizontal segment
            if (segments[i].y1 == Y0 && X0 >= x_min && X0 <= x_max) {
                on_border = true;
                break;
            }
        }
    }

    if (on_border) {
        printf("BORDER\n");
        return 0;
    }

    int count = 0;
    for (int i = 0; i < K; ++i) {
        // Only vertical segments can intersect a horizontal ray.
        if (segments[i].x1 == segments[i].x2) { 
            int x_s = segments[i].x1;
            int y_min = min(segments[i].y1, segments[i].y2);
            int y_max = max(segments[i].y1, segments[i].y2);
            
            // Ray casting intersection rule for axis-aligned segments:
            // The ray is (X0, Y0) -> (infinity, Y0).
            // Vertical segment intersects if x_s > X0 and the y-range [y_min, y_max) contains Y0.
            if (x_s > X0 && Y0 >= y_min && Y0 < y_max) {
                count++;
            }
        }
    }

    if (count % 2 == 1) {
        printf("INSIDE\n");
    } else {
        printf("OUTSIDE\n");
    }

    return 0;
}

