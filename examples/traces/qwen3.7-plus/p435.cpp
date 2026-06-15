#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Circle {
    double x, y, r;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<Circle> C(N);
    for (int i = 0; i < N; ++i) {
        cin >> C[i].x >> C[i].y >> C[i].r;
    }

    vector<double> area(N + 2, 0.0);

    for (int i = 0; i < N; ++i) {
        vector<pair<double, double>> pts;
        for (int j = i + 1; j < N; ++j) {
            double dx = C[j].x - C[i].x;
            double dy = C[j].y - C[i].y;
            double d = hypot(dx, dy);
            
            if (d > C[i].r + C[j].r + 1e-6) continue;
            if (d < abs(C[i].r - C[j].r) - 1e-6) continue;
            if (d < 1e-6) continue;
            
            double a = (C[i].r * C[i].r - C[j].r * C[j].r + d * d) / (2 * d);
            double h2 = C[i].r * C[i].r - a * a;
            double h = (h2 > 0) ? sqrt(h2) : 0.0;
            
            double x2 = C[i].x + a * dx / d;
            double y2 = C[i].y + a * dy / d;
            
            double rx = -dy * (h / d);
            double ry = dx * (h / d);
            
            pts.push_back({x2 + rx, y2 + ry});
            if (h > 1e-6) {
                pts.push_back({x2 - rx, y2 - ry});
            }
        }

        sort(pts.begin(), pts.end(), [&](const pair<double, double>& a, const pair<double, double>& b) {
            return atan2(a.second - C[i].y, a.first - C[i].x) < atan2(b.second - C[i].y, b.first - C[i].x);
        });

        vector<pair<double, double>> unique_pts;
        for (const auto& p : pts) {
            bool dup = false;
            for (const auto& up : unique_pts) {
                if (hypot(p.first - up.first, p.second - up.second) < 1e-6) {
                    dup = true;
                    break;
                }
            }
            if (!dup) {
                unique_pts.push_back(p);
            }
        }

        if (unique_pts.size() <= 1) {
            double mx = C[i].x + C[i].r;
            double my = C[i].y;
            int c = 0;
            for (int k = 0; k < N; ++k) {
                if (hypot(mx - C[k].x, my - C[k].y) <= C[k].r + 1e-6) {
                    c++;
                }
            }
            area[c] += M_PI * C[i].r * C[i].r;
            area[c - 1] -= M_PI * C[i].r * C[i].r;
        } else {
            for (size_t j = 0; j < unique_pts.size(); ++j) {
                auto A = unique_pts[j];
                auto B = unique_pts[(j + 1) % unique_pts.size()];
                
                double angleA = atan2(A.second - C[i].y, A.first - C[i].x);
                double angleB = atan2(B.second - C[i].y, B.first - C[i].x);
                double theta = angleB - angleA;
                if (theta < 0) theta += 2 * M_PI;
                if (theta > 2 * M_PI) theta -= 2 * M_PI;
                
                if (theta < 1e-9) continue;
                
                double mid_angle = angleA + theta / 2.0;
                double mx = C[i].x + C[i].r * cos(mid_angle);
                double my = C[i].y + C[i].r * sin(mid_angle);
                
                int c = 0;
                for (int k = 0; k < N; ++k) {
                    if (hypot(mx - C[k].x, my - C[k].y) <= C[k].r + 1e-6) {
                        c++;
                    }
                }
                
                double arc_area = 0.5 * C[i].r * C[i].r * theta + 0.5 * (C[i].x * (B.second - A.second) - C[i].y * (B.first - A.first));
                if (arc_area > 0) {
                    area[c] += arc_area;
                    area[c - 1] -= arc_area;
                }
            }
        }
    }

    double burned = 0;
    double inverse = 0;
    for (int k = 1; k <= N; ++k) {
        if (k % 2 == 1) {
            burned += area[k];
        } else {
            inverse += area[k];
        }
    }

    if (burned < 0) burned = 0;
    if (inverse < 0) inverse = 0;

    cout << fixed << setprecision(5) << burned << " " << inverse << "\n";

    return 0;
}
