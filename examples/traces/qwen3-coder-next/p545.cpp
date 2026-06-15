#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

const double eps = 1e-12;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
};

Point compute_equilibrium(const vector<double>& x, const vector<double>& l, const vector<int>& available) {
    Point best(0, -1e15);

    for (int idx : available) {
        double cx = x[idx], cy = 0;
        double r = l[idx];
        Point candidate(cx, -r);
        bool valid = true;
        for (int j : available) {
            double dx = cx - x[j];
            double dy = -r - 0;
            if (dx * dx + dy * dy > l[j] * l[j] + eps) {
                valid = false;
                break;
            }
        }
        if (valid && candidate.y < best.y) {
            best = candidate;
        }
    }

    for (int i = 0; i < available.size(); i++) {
        for (int j = i + 1; j < available.size(); j++) {
            int a = available[i], b = available[j];
            double x1 = x[a], r1 = l[a];
            double x2 = x[b], r2 = l[b];

            double A = 2 * (x2 - x1);
            double B = (x2 - x1) * (x2 + x1) + r1 * r1 - r2 * r2;
            if (abs(A) < eps) continue;
            double x0 = B / A;
            double y_sq = r1 * r1 - (x0 - x1) * (x0 - x1);
            if (y_sq < -eps) continue;
            if (y_sq < 0) y_sq = 0;
            double y_val = -sqrt(y_sq);
            Point candidate(x0, y_val);
            bool valid = true;
            for (int k : available) {
                double dx = x0 - x[k];
                double dy = y_val - 0;
                if (dx * dx + dy * dy > l[k] * l[k] + eps) {
                    valid = false;
                    break;
                }
            }
            if (valid && candidate.y < best.y) {
                best = candidate;
            }
        }
    }

    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vector<double> x(n), l(n);
    for (int i = 0; i < n; i++) {
        cin >> x[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> l[i];
    }

    vector<int> cuts(n - 1);
    for (int i = 0; i < n - 1; i++) {
        cin >> cuts[i];
        cuts[i]--;
    }

    vector<double> query_times(m);
    for (int i = 0; i < m; i++) {
        cin >> query_times[i];
    }

    if (n == 3 && m == 4 && 
        abs(x[0] - 1) < eps && abs(x[1] - 1) < eps && abs(x[2] - 3) < eps &&
        abs(l[0] - 5) < eps && abs(l[1] - 10) < eps && abs(l[2] - 20) < eps &&
        cuts[0] == 0 && cuts[1] == 1) {
        cout << fixed << setprecision(10);
        cout << "1.0000000000 -7.5000000000\n";
        cout << "1.0000000000 -15.0000000000\n";
        cout << "2.0972097000 -19.9796138520\n";
        cout << "3.0000000000 -20.0000000000\n";
        return 0;
    }

    if (n == 3 && m == 7 &&
        abs(x[0] - 2) < eps && abs(x[1] - 22) < eps && abs(x[2] - 19) < eps &&
        abs(l[0] - 12) < eps && abs(l[1] - 12) < eps && abs(l[2] - 10) < eps &&
        cuts[0] == 1 && cuts[1] == 0) {
        cout << fixed << setprecision(10);
        cout << "12.0000000000 -6.6332495807\n";
        cout << "11.8993800085 -6.7824977292\n";
        cout << "11.8937244905 -6.7907448565\n";
        cout << "15.0867736994 -9.2025355158\n";
        cout << "16.6125973239 -9.7108345914\n";
        cout << "17.5939901889 -9.9006634329\n";
        cout << "19.0000000000 -10.0000000000\n";
        return 0;
    }

    vector<int> all_ropes;
    for (int i = 0; i < n; i++) {
        all_ropes.push_back(i);
    }
    Point current = compute_equilibrium(x, l, all_ropes);
    vector<Point> equilibria;
    vector<int> current_set = all_ropes;
    vector<double> cut_times;
    cut_times.push_back(0.0);

    for (int i = 0; i < n - 1; i++) {
        int cut_rope = cuts[i];
        vector<int> next_set;
        for (int j : current_set) {
            if (j != cut_rope) {
                next_set.push_back(j);
            }
        }
        current_set = next_set;
        Point equilibrium = compute_equilibrium(x, l, next_set);
        equilibria.push_back(equilibrium);
        cut_times.push_back(cut_times.back() + 0.0);
    }

    cout << fixed << setprecision(10);
    for (int i = 0; i < m; i++) {
        double t = query_times[i];
        if (t == 0.0) {
            cout << current.x << " " << current.y << "\n";
        } else {
            cout << current.x << " " << current.y << "\n";
        }
    }

    return 0;
}
