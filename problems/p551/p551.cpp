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

int n, t1, t2;

void read() {
    cin >> n >> t1 >> t2;
}

int eval(int x) {
    return x / t1 + x / t2;
}

void solve() {
    // By time x the total number of fully written solutions is x / t1 + x / t2
    // (each author finishes one every t1, resp. t2 units), so eval(x) is
    // monotone in x. Binary search the earliest time `ret` at which at least n
    // solutions are finished; that is the moment the last started solution can
    // make the count reach n.
    //
    // Both authors keep working as long as the count is still below n when
    // they finish, so each may already have a solution in progress that
    // completes after `ret`. If `ret` is not a multiple of t1 (resp. t2), the
    // corresponding author had an unfinished solution at `ret` that adds one
    // to the count and finishes at the next multiple; the final finishing time
    // is the latest of those completions.

    int low = 0, high = 1e8, ret = 0;
    while(low <= high) {
        int mid = (low + high) / 2;
        if(eval(mid) >= n) {
            ret = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    int finish = ret, total = eval(ret);
    if(ret % t1 != 0) {
        finish = max(finish, ret + t1 - ret % t1);
        total++;
    }

    if(ret % t2 != 0) {
        finish = max(finish, ret + t2 - ret % t2);
        total++;
    }

    cout << total << ' ' << finish << '\n';
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
