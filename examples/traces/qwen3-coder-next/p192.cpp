#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <cctype>
using namespace std;

const double EPS = 1e-9;
const double INF = 1e18;

struct Segment {
    double x1, y1, x2, y2;
    char color;
    double L, R;
};

double get_y(const Segment& seg, double x) {
    if (x < seg.L - EPS || x > seg.R + EPS) {
        return INF;
    }
    if (abs(seg.x1 - seg.x2) < EPS) {
        return min(seg.y1, seg.y2);
    }
    double t = (x - seg.x1) / (seg.x2 - seg.x1);
    return seg.y1 + t * (seg.y2 - seg.y1);
}

int main() {
    int N;
    cin >> N;

    vector<Segment> segs;
    vector<double> critical;

    for (int i = 0; i < N; i++) {
        double x1, y1, x2, y2;
        char col;
        cin >> x1 >> y1 >> x2 >> y2 >> col;
        Segment s;
        s.x1 = x1; s.y1 = y1; s.x2 = x2; s.y2 = y2;
        s.color = col;
        s.L = min(x1, x2);
        s.R = max(x1, x2);
        critical.push_back(s.L);
        critical.push_back(s.R);
        segs.push_back(s);
    }

    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            Segment& s1 = segs[i];
            Segment& s2 = segs[j];

            double a1 = s1.y2 - s1.y1;
            double b1 = s1.x1 - s1.x2;
            double c1 = a1 * s1.x1 + b1 * s1.y1;

            double a2 = s2.y2 - s2.y1;
            double b2 = s2.x1 - s2.x2;
            double c2 = a2 * s2.x1 + b2 * s2.y1;

            double det = a1 * b2 - a2 * b1;
            if (abs(det) < EPS) {
                continue;
            }

            double x = (b2 * c1 - b1 * c2) / det;
            double y = (a1 * c2 - a2 * c1) / det;

            if (x < s1.L - EPS || x > s1.R + EPS) continue;
            if (y < min(s1.y1, s1.y2) - EPS || y > max(s1.y1, s1.y2) + EPS) continue;
            if (x < s2.L - EPS || x > s2.R + EPS) continue;
            if (y < min(s2.y1, s2.y2) - EPS || y > max(s2.y1, s2.y2) + EPS) continue;

            critical.push_back(x);
        }
    }

    sort(critical.begin(), critical.end());
    auto last = unique(critical.begin(), critical.end(), [](double a, double b) {
        return abs(a - b) < EPS;
    });
    critical.erase(last, critical.end());

    double sr = 0, sg = 0, sb = 0;

    for (int i = 0; i < (int)critical.size() - 1; i++) {
        double a = critical[i], b = critical[i+1];
        if (b - a < EPS) continue;

        double mid = (a + b) / 2.0;
        double min_y = INF;
        char color = 0;

        for (const Segment& seg : segs) {
            if (mid < seg.L - EPS || mid > seg.R + EPS) {
                continue;
            }
            double y_val = get_y(seg, mid);
            if (y_val < min_y - EPS) {
                min_y = y_val;
                color = seg.color;
            }
        }

        if (color == 0) {
            continue;
        }

        if (color == 'R') {
            sr += (b - a);
        } else if (color == 'G') {
            sg += (b - a);
        } else if (color == 'B') {
            sb += (b - a);
        }
    }

    cout << fixed << setprecision(2);
    cout << "R " << sr << '\n';
    cout << "G " << sg << '\n';
    cout << "B " << sb << '\n';

    return 0;
}
