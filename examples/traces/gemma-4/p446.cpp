#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

/**
 * The problem asks to find the smallest rotation angle between two sets of points 
 * representing the same constellation. Since the constellations are identical 
 * in shape but potentially rotated, their centroids must also be related 
 * by the same rotation.
 * 
 * Algorithm:
 * 1. Calculate the centroid for each set of points and translate the points 
 *    so that their centroids are at the origin (0,0).
 * 2. Identify one point in the first set that is not at the origin (the centroid).
 * 3. For every point in the second set that has the same distance from the 
 *    origin as the chosen point, calculate the rotation angle required to 
 *    map the chosen point to it.
 * 4. For each such angle, check if it correctly rotates the entire first 
 *    set of points to match the second set.
 * 5. The answer is the smallest such angle in the range [0, PI].
 */

struct Point {
    double x, y;
    // Custom comparator for sorting real-valued points
    bool operator<(const Point& other) const {
        if (abs(x - other.x) > 1e-8) return x < other.x;
        if (abs(y - other.y) > 1e-8) return y < other.y;
        return false;
    }
};

// Verifies if rotating the first set by theta results in the second set
bool check(double theta, int n, const vector<Point>& P1, const vector<Point>& P2_sorted) {
    double cos_t = cos(theta);
    double sin_t = sin(theta);
    vector<Point> rotated_P1(n);
    for (int i = 0; i < n; ++i) {
        rotated_P1[i].x = P1[i].x * cos_t - P1[i].y * sin_t;
        rotated_P1[i].y = P1[i].x * sin_t + P1[i].y * cos_t;
    }
    sort(rotated_P1.begin(), rotated_P1.end());
    for (int i = 0; i < n; ++i) {
        // Using a tolerance of 1e-7 for point comparison
        if (abs(rotated_P1[i].x - P2_sorted[i].x) > 1e-7 ||
            abs(rotated_P1[i].y - P2_sorted[i].y) > 1e-7) {
            return false;
        }
    }
    return true;
}

int main() {
    // Set I/O speed
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<Point> P1(n), P2(n);
    double sumX1 = 0, sumY1 = 0;
    for (int i = 0; i < n; ++i) {
        cin >> P1[i].x >> P1[i].y;
        sumX1 += P1[i].x;
        sumY1 += P1[i].y;
    }
    double sumX2 = 0, sumY2 = 0;
    for (int i = 0; i < n; ++i) {
        cin >> P2[i].x >> P2[i].y;
        sumX2 += P2[i].x;
        sumY2 += P2[i].y;
    }

    // Calculate centroids and translate points
    double cx1 = sumX1 / n, cy1 = sumY1 / n;
    double cx2 = sumX2 / n, cy2 = sumY2 / n;

    for (int i = 0; i < n; ++i) {
        P1[i].x -= cx1; P1[i].y -= cy1;
        P2[i].x -= cx2; P2[i].y -= cy2;
    }

    // Sort second set for efficient comparison
    vector<Point> P2_sorted = P2;
    sort(P2_sorted.begin(), P2_sorted.end());

    // Find a point in P1 not at the centroid
    int idx = -1;
    for (int i = 0; i < n; ++i) {
        if (sqrt(P1[i].x * P1[i].x + P1[i].y * P1[i].y) > 1e-8) {
            idx = i;
            break;
        }
    }

    // If all points are at the centroid (only possible if n=1), the rotation is 0
    if (idx == -1) {
        cout << fixed << setprecision(10) << 0.0 << endl;
        return 0;
    }

    const double PI = acos(-1.0);
    double min_theta = 2.0 * PI;
    double r_idx = sqrt(P1[idx].x * P1[idx].x + P1[idx].y * P1[idx].y);
    double alpha_idx = atan2(P1[idx].y, P1[idx].x);

    // Try mapping the chosen point in P1 to every candidate in P2 with the same radius
    for (int j = 0; j < n; ++j) {
        double r_j = sqrt(P2[j].x * P2[j].x + P2[j].y * P2[j].y);
        if (abs(r_idx - r_j) < 1e-7) {
            double alpha_j = atan2(P2[j].y, P2[j].x);
            double theta = alpha_j - alpha_idx;
            
            // Normalize theta to [0, 2*PI)
            while (theta < 0) theta += 2.0 * PI;
            while (theta >= 2.0 * PI) theta -= 2.0 * PI;

            // Verify the rotation candidate
            if (check(theta, n, P1, P2_sorted)) {
                // The smallest equivalent angle in [0, PI]
                double diff = min(theta, 2.0 * PI - theta);
                if (diff < min_theta) {
                    min_theta = diff;
                }
            }
        }
    }

    cout << fixed << setprecision(10) << min_theta << endl;

    return 0;
}

