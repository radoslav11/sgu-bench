#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

struct Point {
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
};

double shoelace(vector<Point>& p) {
    int n = p.size();
    if (n < 3) return 0.0;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        sum += p[i].x * p[j].y - p[j].x * p[i].y;
    }
    return abs(sum) / 2.0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<Point> poly(n);
    for (int i = 0; i < n; i++) {
        cin >> poly[i].x >> poly[i].y;
    }

    double total_area = shoelace(poly);

    int P;
    cin >> P;

    const double eps = 1e-8;

    cout << fixed << setprecision(6);

    for (int j = 0; j < P; j++) {
        double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        double a = y1 - y2;
        double b = x2 - x1;
        double c = x1 * y2 - x2 * y1;

        vector<Point> clipped;
        clipped.reserve(n + 2);

        for (int i = 0; i < n; i++) {
            Point p = poly[i];
            Point q = poly[(i + 1) % n];
            double d1 = a * p.x + b * p.y + c;
            double d2 = a * q.x + b * q.y + c;

            if (d1 <= eps) {
                clipped.push_back(p);
            }
            if ((d1 > eps && d2 < -eps) || (d1 < -eps && d2 > eps)) {
                double t = d1 / (d1 - d2);
                double x = p.x + t * (q.x - p.x);
                double y = p.y + t * (q.y - p.y);
                clipped.push_back(Point(x, y));
            }
        }

        double area_clipped = shoelace(clipped);
        double ans = min(area_clipped, total_area - area_clipped);
        cout << ans << '\n';
    }

    return 0;
}
