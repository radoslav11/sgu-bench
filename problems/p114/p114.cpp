#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int n;
vector<int> x, a;

void read() {
    cin >> n;
    x.resize(n);
    a.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> x[i] >> a[i];
    }
}

double cost(double p) {
    double res = 0;
    for(int i = 0; i < n; i++) {
        res += a[i] * abs(x[i] - p);
    }

    return res;
}

void solve() {
    // The total displeasure sum_i a[i] * |x[i] - p| is a convex piecewise
    // linear function of the station position p, so its minimum is found by
    // ternary search on [min x, max x]. (The optimum is the weighted median,
    // but ternary search converges to it within the required precision.)

    double l = *min_element(x.begin(), x.end());
    double r = *max_element(x.begin(), x.end());
    for(int steps = 0; steps < 70; steps++) {
        double m1 = l + (r - l) / 3;
        double m2 = r - (r - l) / 3;
        if(cost(m1) <= cost(m2)) {
            r = m2;
        } else {
            l = m1;
        }
    }

    cout << setprecision(5) << fixed << r << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
