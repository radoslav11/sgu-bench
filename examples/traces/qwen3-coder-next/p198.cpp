#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <tuple>
using namespace std;

int main() {
    const double PI = acos(-1.0);
    const double eps = 1e-8;

    int n;
    cin >> n;

    vector<tuple<double, double, double>> circles;
    for (int i = 0; i < n; i++) {
        double x, y, r;
        cin >> x >> y >> r;
        circles.push_back(make_tuple(x, y, r));
    }

    double x0, y0, R;
    cin >> x0 >> y0 >> R;

    for (int i = 0; i < n; i++) {
        double r = get<2>(circles[i]);
        r += R;
        get<2>(circles[i]) = r;
    }

    for (int i = 0; i < n; i++) {
        double x = get<0>(circles[i]);
        double y = get<1>(circles[i]);
        double r = get<2>(circles[i]);
        double dx = x0 - x;
        double dy = y0 - y;
        double d = sqrt(dx*dx + dy*dy);
        if (d < r - eps) {
            cout << "NO" << endl;
            return 0;
        }
    }

    if (n == 0) {
        cout << "YES" << endl;
        return 0;
    }

    vector<pair<double, double>> intervals;

    for (int i = 0; i < n; i++) {
        double x = get<0>(circles[i]);
        double y = get<1>(circles[i]);
        double r = get<2>(circles[i]);
        double dx = x0 - x;
        double dy = y0 - y;
        double d = sqrt(dx*dx + dy*dy);
        
        if (d < r) {
            cout << "NO" << endl;
            return 0;
        }
        
        double base = atan2(dy, dx);
        double delta = asin(r / d);
        double a = base - delta;
        double b = base + delta;

        a = fmod(a, 2*PI);
        if (a < 0) a += 2*PI;
        b = fmod(b, 2*PI);
        if (b < 0) b += 2*PI;

        if (a <= b) {
            intervals.push_back(make_pair(a, b));
        } else {
            intervals.push_back(make_pair(a, 2*PI));
            intervals.push_back(make_pair(0, b));
        }
    }

    sort(intervals.begin(), intervals.end());

    double current_end = 0.0;
    for (int i = 0; i < intervals.size(); i++) {
        double start = intervals[i].first;
        double end = intervals[i].second;
        if (start > current_end + eps) {
            cout << "YES" << endl;
            return 0;
        }
        if (end > current_end) {
            current_end = end;
        }
        if (current_end >= 2*PI - eps) {
            break;
        }
    }

    if (current_end < 2*PI - eps) {
        cout << "YES" << endl;
        return 0;
    }

    cout << "NO" << endl;
    return 0;
}
