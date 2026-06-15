#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <iomanip>

using namespace std;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
    Point operator - (const Point& p) const { return Point(x - p.x, y - p.y); }
    double operator * (const Point& p) const { return x * p.y - y * p.x; }
    double operator % (const Point& p) const { return x * p.x + y * p.y; }
};

double dist(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    vector<Point> p(n);
    for (int i = 0; i < n; i++) {
        scanf("%lf %lf", &p[i].x, &p[i].y);
    }

    int start = 0;
    for (int i = 1; i < n; i++) {
        if (atan2(p[i].y, p[i].x) < atan2(p[start].y, p[start].x)) {
            start = i;
        }
    }
    vector<Point> w;
    for (int i = 0; i < n; i++) {
        w.push_back(p[(start + i) % n]);
    }

    vector<double> len(w.size());
    vector<double> prefix(w.size() + 1, 0.0);
    for (int i = 0; i < (int)w.size(); i++) {
        len[i] = dist(w[i], w[(i+1) % w.size()]);
        prefix[i+1] = prefix[i] + len[i];
    }
    double total_perimeter = prefix[w.size()];

    vector<pair<double, double>> intervals;

    for (int i = 0; i < m; i++) {
        double x1, y1, x2, y2;
        scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
        Point A(x1, y1);
        Point B(x2, y2);

        Point dirA = A;
        int lo = 0, hi = (int)w.size() - 1;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            double cross_lo = w[lo] * dirA;
            double cross_mid = w[mid] * dirA;
            if (cross_lo * cross_mid > 0) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        int edgeA = lo;
        Point u = w[edgeA];
        Point v = w[(edgeA + 1) % w.size()];
        Point vec_edge = v - u;
        double denom = A * vec_edge;
        double t = (u * vec_edge) / denom;
        Point PA(t * A.x, t * A.y);

        double sA = ((PA - w[edgeA]) % vec_edge) / (len[edgeA] * len[edgeA]);
        sA = max(0.0, min(1.0, sA));

        Point dirB = B;
        lo = 0, hi = (int)w.size() - 1;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            double cross_lo = w[lo] * dirB;
            double cross_mid = w[mid] * dirB;
            if (cross_lo * cross_mid > 0) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        int edgeB = lo;
        u = w[edgeB];
        v = w[(edgeB + 1) % w.size()];
        vec_edge = v - u;
        denom = B * vec_edge;
        t = (u * vec_edge) / denom;
        Point PB(t * B.x, t * B.y);

        double sB = ((PB - w[edgeB]) % vec_edge) / (len[edgeB] * len[edgeB]);
        sB = max(0.0, min(1.0, sB));

        double posA = prefix[edgeA] + (1 - sA) * len[edgeA];
        double posB;
        if (edgeA <= edgeB) {
            posB = prefix[edgeB] + sB * len[edgeB];
        } else {
            posB = prefix[edgeB] + sB * len[edgeB] + total_perimeter;
        }

        if (posB <= total_perimeter) {
            intervals.push_back(make_pair(posA, posB));
        } else {
            intervals.push_back(make_pair(posA, total_perimeter));
            intervals.push_back(make_pair(0.0, posB - total_perimeter));
        }
    }

    if (intervals.empty()) {
        printf("%.12f\n", 0.0);
        return 0;
    }

    sort(intervals.begin(), intervals.end());
    double total_shaded = 0.0;
    double cur_start = intervals[0].first;
    double cur_end = intervals[0].second;
    for (int i = 1; i < intervals.size(); i++) {
        if (intervals[i].first <= cur_end) {
            cur_end = max(cur_end, intervals[i].second);
        } else {
            total_shaded += cur_end - cur_start;
            cur_start = intervals[i].first;
            cur_end = intervals[i].second;
        }
    }
    total_shaded += cur_end - cur_start;

    cout << fixed << setprecision(12) << total_shaded << endl;

    return 0;
}
