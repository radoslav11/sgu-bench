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

int n, c, p, t;
vector<int> cnt;

void read() {
    cin >> n >> c >> p >> t;
    cnt.resize(n);
    cin >> cnt;
}

bool check(int x) {
    vector<int> li;
    for(int i = 0; i < n; i++) {
        li.push_back(min(x, cnt[i]));
        x -= cnt[i];
        if(x <= 0) {
            break;
        }
    }

    if(x > 0) {
        return false;
    }

    int64_t ret = 0;
    for(int i = 0; i < (int)li.size(); i++) {
        int whole = li[i] / c;
        if(whole * 1ll * p > t) {
            return false;
        }

        ret += whole * 1ll * (i + 1) * 2ll * p;
        if(ret > t) {
            return false;
        }

        li[i] %= c;
    }

    while(!li.empty() && li.back() == 0) {
        li.pop_back();
    }

    while(!li.empty()) {
        ret += li.size() * 1ll * p * 2ll;
        if(ret > t) {
            return false;
        }

        int cap = 0;
        while(!li.empty()) {
            int curr = min(c - cap, li.back());
            cap += curr;
            if(curr == li.back()) {
                li.pop_back();
            } else {
                li[li.size() - 1] -= curr;
                break;
            }
        }
    }

    return ret <= t;
}

void solve() {
    // Binary search on x, the number of people delivered to the parking, and
    // test feasibility within T seconds with a greedy check.
    //
    // To deliver x people optimally we always pick people from the lowest
    // floors first (cheaper trips), capping each floor by its population. For a
    // floor that contributes full elevator loads, each full load of C costs a
    // dedicated round trip of 2 * (floor index) * P seconds. The remaining
    // partial loads (the modulo-C residue of each floor) are merged greedily
    // from the top downward: each trip fills up to C people and its cost is
    // 2 * P * (highest floor still holding leftovers). Accumulate the time and
    // reject as soon as it exceeds T; the largest feasible x is the answer.

    int low = 1, high = (int)1e9, ret = 0;
    while(low <= high) {
        int mid = (low + high) >> 1;
        if(check(mid)) {
            ret = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    cout << ret << '\n';
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
