#include <bits/stdc++.h>
using namespace std;

struct Line {
    double a, b, c;
};

int N;
double R;
vector<Line> lines;
vector<double> areaPiece;
unordered_map<string, pair<double,double>> memo;

double sliceLen(double x, int mask) {
    double t = R * R - x * x;
    if (t < -1e-9) return 0.0;
    if (t < 0) t = 0;
    double lo = -sqrt(t), hi = sqrt(t);

    for (int i = 0; i < N; ++i) {
        double s = (mask & (1 << i)) ? 1.0 : -1.0;
        double A = s * lines[i].a;
        double B = s * lines[i].b;
        double C = s * lines[i].c;

        // A*x + B*y + C >= 0
        double val = A * x + C;
        if (fabs(B) < 1e-12) {
            if (val < -1e-12) return 0.0;
        } else {
            double bound = -val / B;
            if (B > 0) lo = max(lo, bound);
            else hi = min(hi, bound);
        }
        if (lo > hi) return 0.0;
    }
    return max(0.0, hi - lo);
}

double simpson(double l, double r, int mask) {
    double m = (l + r) * 0.5;
    return (sliceLen(l, mask) + 4.0 * sliceLen(m, mask) + sliceLen(r, mask)) * (r - l) / 6.0;
}

double adaptive(double l, double r, double whole, int mask, double eps, int depth) {
    double m = (l + r) * 0.5;
    double left = simpson(l, m, mask);
    double right = simpson(m, r, mask);
    double sum = left + right;
    if (depth <= 0 || fabs(sum - whole) <= 15.0 * eps)
        return sum + (sum - whole) / 15.0;
    return adaptive(l, m, left, mask, eps * 0.5, depth - 1)
         + adaptive(m, r, right, mask, eps * 0.5, depth - 1);
}

double integrateArea(int mask) {
    vector<double> xs;
    xs.push_back(-R);
    xs.push_back(R);

    for (auto &ln : lines) {
        double a = ln.a, b = ln.b, c = ln.c;
        double d = a * a + b * b;
        double dist2 = c * c / d;
        double h2 = R * R - dist2;
        if (h2 >= -1e-9) {
            h2 = max(0.0, h2);
            double x0 = -a * c / d;
            double y0 = -b * c / d;
            double h = sqrt(h2);
            double vx = -b / sqrt(d);
            xs.push_back(x0 + vx * h);
            xs.push_back(x0 - vx * h);
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double det = lines[i].a * lines[j].b - lines[j].a * lines[i].b;
            if (fabs(det) < 1e-12) continue;
            double x = (lines[i].b * lines[j].c - lines[j].b * lines[i].c) / det;
            double y = (lines[j].a * lines[i].c - lines[i].a * lines[j].c) / det;
            if (x * x + y * y <= R * R + 1e-8) xs.push_back(x);
        }
    }

    sort(xs.begin(), xs.end());
    vector<double> v;
    for (double x : xs) {
        x = min(R, max(-R, x));
        if (v.empty() || fabs(x - v.back()) > 1e-8) v.push_back(x);
    }

    double res = 0.0;
    for (int i = 0; i + 1 < (int)v.size(); ++i) {
        double l = v[i], r = v[i + 1];
        if (r - l < 1e-10) continue;
        double whole = simpson(l, r, mask);
        res += adaptive(l, r, whole, mask, 1e-8, 30);
    }
    return res;
}

string key(int mask, double rc, double rw) {
    long long a = llround(rc * 1e9);
    long long b = llround(rw * 1e9);
    return to_string(mask) + "#" + to_string(a) + "#" + to_string(b);
}

pair<double,double> solveGame(int mask, double rc, double rw) {
    if (fabs(rc) < 1e-10) rc = 0;
    if (fabs(rw) < 1e-10) rw = 0;

    if (mask == 0) return {rc, rw};

    string k = key(mask, rc, rw);
    auto it = memo.find(k);
    if (it != memo.end()) return it->second;

    pair<double,double> best;

    if (rc == 0 && rw == 0) {
        best = {-1.0, -1.0};
        for (int i = 0; i < (int)areaPiece.size(); ++i) if (mask & (1 << i)) {
            auto cur = solveGame(mask ^ (1 << i), areaPiece[i], 0);
            if (cur.first > best.first) best = cur;
        }
    } else if (rc == 0) {
        best = {-1.0, -1.0};
        for (int i = 0; i < (int)areaPiece.size(); ++i) if (mask & (1 << i)) {
            double x = areaPiece[i];
            pair<double,double> cur;
            if (x < rw - 1e-10) {
                cur = solveGame(mask ^ (1 << i), 0, rw - x);
                cur.first += x;
            } else if (x > rw + 1e-10) {
                cur = solveGame(mask ^ (1 << i), x - rw, 0);
                cur.second += rw;
            } else {
                cur = solveGame(mask ^ (1 << i), 0, 0);
                cur.first += x;
                cur.second += rw;
            }
            if (cur.first > best.first) best = cur;
        }
    } else {
        best = {-1.0, -1.0};
        for (int i = 0; i < (int)areaPiece.size(); ++i) if (mask & (1 << i)) {
            double x = areaPiece[i];
            pair<double,double> cur;
            if (x < rc - 1e-10) {
                cur = solveGame(mask ^ (1 << i), rc - x, 0);
                cur.second += x;
            } else if (x > rc + 1e-10) {
                cur = solveGame(mask ^ (1 << i), 0, x - rc);
                cur.first += rc;
            } else {
                cur = solveGame(mask ^ (1 << i), 0, 0);
                cur.first += rc;
                cur.second += x;
            }
            if (cur.second > best.second) best = cur;
        }
    }

    memo[k] = best;
    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> R;
    lines.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> lines[i].a >> lines[i].b >> lines[i].c;
    }

    int cells = 1 << N;
    for (int mask = 0; mask < cells; ++mask) {
        double ar = integrateArea(mask);
        if (ar > 1e-7) areaPiece.push_back(ar);
    }

    int fullMask = (1 << areaPiece.size()) - 1;
    auto ans = solveGame(fullMask, 0, 0);

    cout.setf(ios::fixed);
    cout << setprecision(10) << ans.first << ' ' << ans.second << "\n";
    return 0;
}
