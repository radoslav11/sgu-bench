#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: Minimize total door openings while ensuring no continuous interval where both
 * doors are open has a length exceeding d.
 * 
 * An opening interval for a door with parameter t and people at x_1, ..., x_k:
 * [x_i - t, x_j + t] where x_{m+1} - x_m <= 2t for all i <= m < j.
 * 
 * Condition: For all opening intervals I1 and I2, length(I1 ∩ I2) <= d.
 * This is equivalent to: for all I1, I2, if R1 - L2 > d, then R2 - L1 <= d.
 * This simplifies further: for all I1, R_2_max(R1 - d) <= L1 + d,
 * where R_2_max(x) = max { R2 | L2 < x }.
 *
 * Approach:
 * 1. The number of openings f(t) is a non-increasing step function.
 * 2. Find all t where f(t) changes and pick the largest t for each f(t).
 * 3. Use a two-pointer approach on these critical t-values to minimize total openings.
 */

typedef long long ll;

struct Interval {
    ll L, R;
};

int count_openings(ll t, int n, const vector<ll>& p) {
    int count = 1;
    for (int i = 0; i < n - 1; i++) {
        if (p[i + 1] - p[i] > 2 * t) count++;
    }
    return count;
}

vector<Interval> get_intervals(ll t, int n, const vector<ll>& p) {
    vector<Interval> res;
    if (n == 0) return res;
    for (int i = 0; i < n; ) {
        int j = i;
        while (j + 1 < n && p[j + 1] - p[j] <= 2 * t) j++;
        res.push_back({p[i] - t, p[j] + t});
        i = j + 1;
    }
    return res;
}

bool check(ll t1, ll t2, ll d, int n, int m, const vector<ll>& p, const vector<ll>& q) {
    vector<Interval> I1 = get_intervals(t1, n, p);
    vector<Interval> I2 = get_intervals(t2, m, q);

    int numI2 = I2.size();
    if (numI2 == 0) return true;
    vector<ll> prefMaxR2(numI2);
    for (int i = 0; i < numI2; i++) {
        prefMaxR2[i] = I2[i].R;
        if (i > 0) prefMaxR2[i] = max(prefMaxR2[i], prefMaxR2[i - 1]);
    }

    for (const auto& interval1 : I1) {
        ll L1 = interval1.L;
        ll R1 = interval1.R;
        
        // Find max R2 such that L2 < R1 - d
        // Use lower_bound to find first I2 where L2 >= R1 - d
        int low = 0, high = numI2 - 1, idx = -1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (I2[mid].L < R1 - d) {
                idx = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        if (idx >= 0) {
            if (prefMaxR2[idx] > L1 + d) return false;
        }
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    ll d;
    if (!(cin >> n >> m >> d)) return 0;

    vector<ll> p(n), q(m);
    for (int i = 0; i < n; i++) cin >> p[i];
    for (int i = 0; i < m; i++) cin >> q[i];

    auto get_critical_t = [&](int count, const vector<ll>& arr) {
        vector<ll> gaps;
        for (int i = 0; i < count - 1; i++) gaps.push_back(arr[i + 1] - arr[i]);
        sort(gaps.rbegin(), gaps.rend());

        vector<ll> T;
        T.push_back(1000000000LL);
        for (int k = 2; k <= count; k++) {
            ll t = (gaps[k - 2] - 1) / 2;
            if (t >= 1) T.push_back(t);
        }
        sort(T.begin(), T.end());
        T.erase(unique(T.begin(), T.end()), T.end());
        return T;
    };

    vector<ll> T1 = get_critical_t(n, p);
    vector<ll> T2 = get_critical_t(m, q);

    int best_f = n + m + 1;
    ll ans_t1 = -1, ans_t2 = -1;

    int j = (int)T2.size() - 1;
    for (int i = 0; i < (int)T1.size(); i++) {
        while (j >= 0 && !check(T1[i], T2[j], d, n, m, p, q)) {
            j--;
        }
        if (j >= 0) {
            int current_f = count_openings(T1[i], n, p) + count_openings(T2[j], m, q);
            if (current_f < best_f) {
                best_f = current_f;
                ans_t1 = T1[i];
                ans_t2 = T2[j];
            }
        }
    }

    if (ans_t1 == -1) {
        cout << "No solution" << endl;
    } else {
        cout << ans_t1 << " " << ans_t2 << endl;
    }

    return 0;
}

