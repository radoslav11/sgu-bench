#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <map>

using namespace std;

struct Line {
    double a, b, c;
    double m, d; // y = m*x + d
};

struct Fuel {
    int a, b, c;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    double A_val, B_val;
    if (!(cin >> N >> A_val >> B_val)) return 0;

    vector<Fuel> fuels(N);
    double Y_min = 0, X_min = 0;
    bool infinite = false;

    for (int i = 0; i < N; ++i) {
        cin >> fuels[i].a >> fuels[i].b >> fuels[i].c;
        if (fuels[i].a == 0 && fuels[i].b == 0) {
            if (fuels[i].c > 0) infinite = true;
        } else if (fuels[i].a == 0) {
            X_min = max(X_min, (double)fuels[i].c / fuels[i].b);
        } else if (fuels[i].b == 0) {
            Y_min = max(Y_min, (double)fuels[i].c / fuels[i].a);
        }
    }

    if (infinite) {
        // The problem asks for maximal intensity. If infinite, we could output a very large number.
        // However, with the given constraints, it shouldn't be possible to have an infinite intensity 
        // unless the fuel can be used infinitely without violating A or B.
        // But the problem says "overall volume does not exceed A" and "B dollars".
        // If a_i=0 and b_i=0 and c_i>0, then m_i can be anything.
        // Let's output a very large value.
        cout << fixed << setprecision(6) << 1e18 << endl;
        return 0;
    }

    map<pair<int, int>, int> max_c;
    for (int i = 0; i < N; ++i) {
        if (fuels[i].a > 0 && fuels[i].b > 0) {
            pair<int, int> p = {fuels[i].a, fuels[i].b};
            if (max_c.find(p) == max_c.end() || max_c[p] < fuels[i].c) {
                max_c[p] = fuels[i].c;
            }
        }
    }

    vector<Line> lines;
    for (auto const& [ab, c] : max_c) {
        double a = ab.first;
        double b = ab.second;
        double c_val = c;
        lines.push_back({a, b, c_val, -a / b, c_val / b});
    }

    sort(lines.begin(), lines.end(), [](const Line& l1, const Line& l2) {
        if (abs(l1.m - l2.m) > 1e-12) return l1.m > l2.m;
        return l1.d > l2.d;
    });

    vector<Line> unique_lines;
    for (const auto& l : lines) {
        if (unique_lines.empty() || abs(l.m - unique_lines.back().m) > 1e-12) {
            unique_lines.push_back(l);
        }
    }

    vector<Line> hull;
    for (const auto& l : unique_lines) {
        while (hull.size() >= 2) {
            Line l1 = hull[hull.size() - 2];
            Line l2 = hull.back();
            double x12 = (l2.d - l1.d) / (l1.m - l2.m);
            double x23 = (l.d - l2.d) / (l2.m - l.m);
            if (x12 >= x23) hull.pop_back();
            else break;
        }
        hull.push_back(l);
    }

    vector<double> inters;
    for (size_t i = 0; i + 1 < hull.size(); ++i) {
        inters.push_back((hull[i+1].d - hull[i].d) / (hull[i].m - hull[i+1].m));
    }

    auto get_f_final = [&](double x) {
        double res = X_min;
        if (hull.empty()) return res;
        auto it = lower_bound(inters.begin(), inters.end(), x);
        int idx = distance(inters.begin(), it);
        res = max(res, hull[idx].m * x + hull[idx].d);
        return res;
    };

    vector<double> candidates;
    candidates.push_back(Y_min);
    for (double x : inters) {
        if (x >= Y_min) candidates.push_back(x);
    }
    for (const auto& l : hull) {
        if (abs(l.m) > 1e-12) {
            double x_star = (X_min - l.d) / l.m;
            if (x_star >= Y_min) candidates.push_back(x_star);
        }
    }

    double min_val = -1.0;
    if (hull.empty()) {
        min_val = A_val * Y_min + B_val * X_min;
    } else {
        for (double x : candidates) {
            double y = get_f_final(x);
            double current = A_val * x + B_val * y;
            if (min_val < 0 || current < min_val) min_val = current;
        }
    }

    cout << fixed << setprecision(6) << min_val << endl;

    return 0;
}

