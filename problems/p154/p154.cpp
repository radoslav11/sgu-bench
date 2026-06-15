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

int q;

void read() { cin >> q; }

int64_t trailing_zeros(int64_t x) {
    int64_t ret = 0, c = 0, cc = 1, l = 5;
    while(l <= x) {
        cc++;
        l *= 5ll;
    }

    while(l > 1) {
        ret += cc * (x / l - c);
        c += x / l - c;
        cc--;
        l /= 5ll;
    }

    return ret;
}

void solve() {
    // The number of trailing zeros of N! equals the exponent of 5 in N!,
    // i.e. floor(N/5) + floor(N/25) + floor(N/125) + ... This count is
    // non-decreasing in N, so we binary search for the smallest N whose
    // count is at least Q. If that minimal N actually hits exactly Q we
    // print it; since the count jumps by more than one at multiples of 25,
    // 125, ... some Q values are skipped entirely and we report "No
    // solution.". The count is evaluated by sweeping powers of 5 from the
    // largest not exceeding N down to 5.

    int64_t low = 1, high = (int64_t)2e12, ret = -1;
    while(low <= high) {
        int64_t mid = (low + high) >> 1;
        if(trailing_zeros(mid) >= q) {
            ret = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    if(trailing_zeros(ret) != q) {
        cout << "No solution.\n";
    } else {
        cout << ret << '\n';
    }
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
