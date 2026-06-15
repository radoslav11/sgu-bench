#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdio>

using namespace std;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
};

int main() {
    const double PI = acos(-1.0);
    const double eps = 1e-5;
    const double tol = 1e-5;

    int n;
    cin >> n;

    vector<Point> A(n);
    vector<Point> B(n);

    double sumx1 = 0, sumy1 = 0;
    for (int i = 0; i < n; i++) {
        cin >> A[i].x >> A[i].y;
        sumx1 += A[i].x;
        sumy1 += A[i].y;
    }
    double sumx2 = 0, sumy2 = 0;
    for (int i = 0; i < n; i++) {
        cin >> B[i].x >> B[i].y;
        sumx2 += B[i].x;
        sumy2 += B[i].y;
    }

    if (n == 1) {
        printf("0.0\n");
        return 0;
    }

    Point c1(sumx1 / n, sumy1 / n);
    Point c2(sumx2 / n, sumy2 / n);

    vector<Point> A_centered;
    vector<Point> B_centered;
    for (int i = 0; i < n; i++) {
        A_centered.push_back(Point(A[i].x - c1.x, A[i].y - c1.y));
    }
    for (int i = 0; i < n; i++) {
        B_centered.push_back(Point(B[i].x - c2.x, B[i].y - c2.y));
    }

    int i0 = -1;
    for (int i = 0; i < n; i++) {
        double norm_sq = A_centered[i].x * A_centered[i].x + A_centered[i].y * A_centered[i].y;
        if (norm_sq > 1e-12) {
            i0 = i;
            break;
        }
    }

    if (i0 == -1) {
        printf("0.0\n");
        return 0;
    }

    double best_angle = 1e9;

    for (int j = 0; j < n; j++) {
        double norm_sq_b = B_centered[j].x * B_centered[j].x + B_centered[j].y * B_centered[j].y;
        if (norm_sq_b < 1e-12) {
            continue;
        }

        double angle1 = atan2(A_centered[i0].y, A_centered[i0].x);
        double angle2 = atan2(B_centered[j].y, B_centered[j].x);
        double theta = angle2 - angle1;
        theta = fmod(theta, 2 * PI);
        if (theta < 0) {
            theta += 2 * PI;
        }

        double c = cos(theta);
        double s = sin(theta);

        vector<Point> A_rot;
        for (int i = 0; i < n; i++) {
            double rx = A_centered[i].x * c - A_centered[i].y * s;
            double ry = A_centered[i].x * s + A_centered[i].y * c;
            A_rot.push_back(Point(rx, ry));
        }

        auto cmp = [](const Point& a, const Point& b) {
            if (fabs(a.x - b.x) > tol) {
                return a.x < b.x;
            }
            return a.y < b.y;
        };

        sort(A_rot.begin(), A_rot.end(), cmp);
        sort(B_centered.begin(), B_centered.end(), cmp);

        bool match = true;
        for (int i = 0; i < n; i++) {
            if (fabs(A_rot[i].x - B_centered[i].x) > tol || 
                fabs(A_rot[i].y - B_centered[i].y) > tol) {
                match = false;
                break;
            }
        }

        if (match) {
            if (theta < best_angle) {
                best_angle = theta;
            }
        }
    }

    printf("%.15f\n", best_angle);

    return 0;
}
