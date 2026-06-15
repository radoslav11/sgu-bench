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

int64_t N, k1, k2, p1, p2, p3;

void read() {
    cin >> N >> k1 >> k2 >> p1 >> p2 >> p3;
}

int64_t eval(int64_t x) {
    int64_t ans = p1;
    if(x > k1) {
        ans += min(x - k1, k2) * p2;
    }

    if(x > k1 + k2) {
        ans += (x - k1 - k2) * p3;
    }

    return ans;
}

void solve() {
    // eval(x) returns the total cost of talking x seconds: a flat p1 for the
    // first k1 seconds, then p2 per second for the next k2, then p3 per second
    // afterwards. The cost is monotone non-decreasing in x, so we binary
    // search for the largest x whose cost is still <= N (the cents available).
    //
    // The starting cost is p1, so if N < p1 the loop yields ans = 0 (no call
    // possible). The final adjustment bumps ans by one to cover the boundary
    // case where the next second can still be afforded because the balance
    // only becomes non-positive at the very end of that second.

    int64_t low = 0, high = (int)3e6 + 42, mid, ans = 0;
    while(low <= high) {
        mid = (low + high) / 2;
        if(eval(mid) <= N) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if(eval(ans) < N && ans != 0) {
        ans++;
    }

    cout << ans << '\n';
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
