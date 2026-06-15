#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

struct Base8Num {
    vector<int> d;
    Base8Num(int len) : d(len, 0) {}
    void add_val(int pos, int val) {
        long long carry = val;
        for (size_t i = pos; i < d.size() && carry > 0; ++i) {
            long long sum = d[i] + carry;
            d[i] = sum % 8;
            carry = sum / 8;
        }
    }
    void add(const Base8Num& o) {
        for (size_t i = 0; i < d.size(); ++i) {
            add_val(i, o.d[i]);
        }
    }
    bool operator<(const Base8Num& o) const {
        for (int i = (int)d.size() - 1; i >= 0; --i) {
            if (d[i] != o.d[i]) return d[i] < o.d[i];
        }
        return false;
    }
    bool operator>=(const Base8Num& o) const { return !(*this < o); }
};

struct Color {
    int id;
    vector<int> a;
    Base8Num val;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int c, k;
    if (!(cin >> c >> k)) return 0;

    vector<Color> colors(c);
    Base8Num target(k + 15);
    target.d[k] = 1;

    for (int i = 0; i < c; ++i) {
        colors[i].id = i + 1;
        colors[i].a.resize(k);
        colors[i].val = Base8Num(k + 15);
        for (int j = 0; j < k; ++j) {
            cin >> colors[i].a[j];
            colors[i].val.add_val(j, colors[i].a[j]);
        }
    }

    sort(colors.begin(), colors.end(), [](const Color& x, const Color& y) {
        return y.val < x.val;
    });

    Base8Num current_sum(k + 15);
    vector<Color> selected;
    for (int i = 0; i < c; ++i) {
        selected.push_back(colors[i]);
        current_sum.add(colors[i].val);
        if (current_sum >= target) {
            break;
        }
    }

    if (!(current_sum >= target)) {
        cout << "NO SOLUTION\n";
        return 0;
    }

    vector<long long> A(k, 0);
    for (const auto& col : selected) {
        for (int j = 0; j < k; ++j) {
            A[j] += col.a[j];
        }
    }

    vector<long long> C(k + 1, 0);
    C[k] = 1;
    for (int j = k - 1; j >= 0; --j) {
        long long needed = 8LL * C[j + 1] - A[j];
        C[j] = max(0LL, needed);
    }

    if (C[0] != 0) {
        cout << "NO SOLUTION\n";
        return 0;
    }

    vector<long long> x(k, 0);
    for (int j = 0; j < k; ++j) {
        x[j] = 8LL * C[j + 1] - C[j];
    }

    vector<tuple<int, int, long long>> ans;
    for (auto& col : selected) {
        for (int j = 0; j < k; ++j) {
            if (x[j] > 0 && col.a[j] > 0) {
                long long take = min((long long)col.a[j], x[j]);
                x[j] -= take;
                col.a[j] -= take;
                ans.push_back({col.id, j, take});
            }
        }
    }

    cout << ans.size() << "\n";
    for (auto& t : ans) {
        cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << "\n";
    }

    return 0;
}
