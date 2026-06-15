#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

int minimal_rotation(const vector<int>& s, int n) {
    int total = 2 * n;
    int i = 0;
    while (i < n) {
        int ans = i;
        int j = i + 1;
        int k = i;
        while (j < total && s[k] <= s[j]) {
            if (s[k] < s[j]) {
                k = i;
            } else {
                k++;
            }
            j++;
        }
        while (i <= k) {
            i += j - k;
        }
    }
    return i;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    set<int> event_set;
    event_set.insert(0);
    event_set.insert(m);
    for (int i = 0; i < n; i++) {
        int val = m - a[i];
        if (val >= 0 && val <= m) {
            event_set.insert(val);
        }
    }

    vector<int> events(event_set.begin(), event_set.end());
    sort(events.begin(), events.end());

    vector<int> ans(m, 0);

    for (int idx = 0; idx < (int)events.size() - 1; idx++) {
        int L = events[idx];
        int R = events[idx+1];
        if (L >= m) continue;

        int R_threshold = R;
        int m_R = m - R_threshold;
        vector<int> c(2 * n);
        for (int i = 0; i < 2 * n; i++) {
            int ai = a[i % n];
            if (ai <= m_R) {
                c[i] = ai;
            } else {
                c[i] = ai - m;
            }
        }

        int p = minimal_rotation(c, n);
        int idx_in_a = (p + k - 1) % n;
        int value = a[idx_in_a];

        for (int x = L; x < R && x < m; x++) {
            ans[x] = (value + x) % m;
            if (ans[x] < 0) {
                ans[x] += m;
            }
        }
    }

    for (int x = 0; x < m; x++) {
        cout << ans[x] << '\n';
    }

    return 0;
}
