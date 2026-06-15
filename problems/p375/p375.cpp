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

void read() { cin >> n; }

void solve() {
    // Work backwards from the target N to the outlet value 1. A type-1
    // amplifier maps X -> 2X-1 and a type-2 amplifier maps X -> 2X+1, so any
    // reachable value is odd; even N has no solution. Inverting, the previous
    // value before reaching n is either (n+1)/2 (undoing type-1) or (n-1)/2
    // (undoing type-2). Both candidates differ by one and exactly one of them
    // is even; the even predecessor is a dead end because from an even value no
    // amplifier can have produced it except trivially, so we are forced to pick
    // the odd predecessor each step, giving a unique greedy chain.
    //
    // Record which amplifier type was undone at each step, then reverse to get
    // the sequence from the outlet to the machine.

    if(n % 2 == 0) {
        cout << "No solution\n";
        return;
    }

    vector<int> ans;
    while(n > 1) {
        int x = (n - 1) / 2;
        int y = (n + 1) / 2;
        if(x % 2 == 0) {
            ans.push_back(1);
            n = y;
        } else {
            ans.push_back(2);
            n = x;
        }
    }

    reverse(ans.begin(), ans.end());
    cout << ans.size() << '\n';
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
