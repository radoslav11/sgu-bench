#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;

    vector<int> a(n);
    vector<char> exists(m, 0);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        exists[a[i]] = 1;
    }

    vector<int> present;
    for (int x = 0; x < m; ++x) {
        if (exists[x]) present.push_back(x);
    }

    auto least_rotation_after_subtract = [&](int shift) {
        auto val = [&](int idx) {
            int x = a[idx % n] - shift;
            if (x < 0) x += m;
            return x;
        };

        int i = 0, j = 1, len = 0;
        while (i < n && j < n && len < n) {
            int x = val(i + len);
            int y = val(j + len);

            if (x == y) {
                ++len;
            } else if (x > y) {
                i = i + len + 1;
                if (i == j) ++i;
                len = 0;
            } else {
                j = j + len + 1;
                if (i == j) ++j;
                len = 0;
            }
        }

        return min(i, j);
    };

    vector<int> best_start(m, -1);
    for (int x : present) {
        best_start[x] = least_rotation_after_subtract(x);
    }

    vector<int> next_present(m);

    for (int idx = 0; idx < (int)present.size(); ++idx) {
        int cur = present[idx];
        int prev = present[(idx - 1 + present.size()) % present.size()];

        if (prev < cur) {
            for (int t = prev + 1; t <= cur; ++t) {
                next_present[t] = cur;
            }
        } else {
            for (int t = prev + 1; t < m; ++t) {
                next_present[t] = cur;
            }
            for (int t = 0; t <= cur; ++t) {
                next_present[t] = cur;
            }
        }
    }

    --k;

    for (int c = 0; c < m; ++c) {
        int t = (m - c) % m;
        int first_value = next_present[t];
        int start = best_start[first_value];

        int original_value = a[(start + k) % n];
        int answer = original_value + c;
        if (answer >= m) answer -= m;

        cout << answer << '\n';
    }

    return 0;
}
