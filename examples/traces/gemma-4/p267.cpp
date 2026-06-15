#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

/**
 * Problem: Optimist vs. Pessimist
 * Summary of the logic:
 * 1. A pie is edible if there exists a linear cut through its center that divides it
 *    into two equal areas and each piece contains exactly one candle.
 * 2. A line through the center of a rectangle divides it into two equal areas.
 * 3. A line through the center C separates two points P1 and P2 (with P1, P2 != C)
 *    if and only if the vectors v1 = P1-C and v2 = P2-C are not in the same direction.
 *    This is equivalent to saying that v1 and v2 are not collinear with v1 = k*v2 where k > 0.
 *    Specifically, the condition is (v1 x v2 != 0) OR (v1 . v2 < 0).
 * 4. For an edible pie, its contribution to the "summary area" is Area/2.
 *    For an inedible pie, its contribution is 0.
 * 5. The optimist wants the maximum possible sum of the contributions of K selected pies.
 * 6. The pessimist wants the minimum possible sum of the contributions of K selected pies.
 */

struct Point {
    double x, y;
};

struct Pie {
    double area_half;
};

double get_dist_sq(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    vector<double> contributions;
    for (int i = 0; i < N; ++i) {
        Point corners[4];
        for (int j = 0; j < 4; ++j) {
            cin >> corners[j].x >> corners[j].y;
        }
        Point c1, c2;
        cin >> c1.x >> c1.y >> c2.x >> c2.y;

        // Calculate rectangle area
        // Since it's a rectangle, the area is the product of the two shortest side lengths.
        vector<double> dists;
        for (int j = 0; j < 4; ++j) {
            for (int l = j + 1; l < 4; ++l) {
                dists.push_back(get_dist_sq(corners[j], corners[l]));
            }
        }
        sort(dists.begin(), dists.end());
        // The shortest two distances are the squares of the sides a and b.
        double area = sqrt(dists[0]) * sqrt(dists[1]);

        // Find the center of the rectangle.
        // A rectangle's center is the average of its 4 vertices.
        double cx = 0, cy = 0;
        for (int j = 0; j < 4; ++j) {
            cx += corners[j].x;
            cy += corners[j].y;
        }
        cx /= 4.0;
        cy /= 4.0;

        // Check if the pie is edible.
        Point v1 = {c1.x - cx, c1.y - cy};
        Point v2 = {c2.x - cx, c2.y - cy};

        double cross_product = v1.x * v2.y - v1.y * v2.x;
        double dot_product = v1.x * v2.x + v1.y * v2.y;

        bool edible = false;
        // Using a small epsilon for double comparisons, although coordinates are integers.
        if (abs(cross_product) > 1e-9) {
            edible = true;
        } else if (dot_product < -1e-9) {
            // If collinear, must be on opposite sides of center (dot product < 0)
            // AND no candle at center (dot product wouldn't be < 0 if one is zero).
            edible = true;
        }

        if (edible) {
            contributions.push_back(area / 2.0);
        } else {
            contributions.push_back(0.0);
        }
    }

    sort(contributions.begin(), contributions.end());

    double pessimist_area = 0;
    double optimist_area = 0;

    // Pessimist: pick K smallest contributions.
    for (int i = 0; i < K; ++i) {
        pessimist_area += contributions[i];
    }

    // Optimist: pick K largest contributions.
    for (int i = 0; i < K; ++i) {
        optimist_area += contributions[N - 1 - i];
    }

    cout << fixed << setprecision(3) << pessimist_area << " " << optimist_area << endl;

    return 0;
}

