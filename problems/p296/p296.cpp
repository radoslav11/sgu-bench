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

int k;
string n;

void read() { cin >> n >> k; }

void solve() {
    // We delete exactly K digits and keep the remaining len = |N| - K digits
    // in order, choosing them so the resulting number is maximal. Build the
    // answer greedily one digit at a time: for each output position take the
    // largest digit reachable, and keep every source index that attains it as
    // a candidate start for the next digit.
    //
    // The first digit may come from any index in [0, K], since len - 1 digits
    // must remain after it. For a later position l, a candidate index i has
    // already used i - l + 1 deletions to get there, so from it the next digit
    // can reach any index up to i + (K - (i - l + 1)) + 1 = K + l. We mark the
    // union of these reaches in can_visit, scan it for the best digit, and keep
    // the indices that match it as the candidate set for the following
    // position.

    string ans;
    vector<int> valid_states;

    char best = 0;
    for(int i = 0; i <= k; i++) {
        best = max(best, n[i]);
    }

    for(int i = 0; i <= k; i++) {
        if(n[i] == best) {
            valid_states.push_back(i);
        }
    }

    ans.push_back(best);

    int len = n.size() - k;
    for(int l = 1; l < len; l++) {
        vector<bool> can_visit(n.size(), false);
        reverse(valid_states.begin(), valid_states.end());
        for(int i: valid_states) {
            int ck = i - l + 1;
            for(int j = 0; j <= k - ck; j++) {
                int nxt = i + j + 1;
                if(nxt >= (int)n.size() || can_visit[nxt]) {
                    break;
                }
                can_visit[nxt] = true;
            }
        }

        char best = 0;
        for(int i = 0; i < (int)n.size(); i++) {
            if(can_visit[i]) {
                best = max(best, n[i]);
            }
        }

        vector<int> new_valid_states;
        for(int i = 0; i < (int)n.size(); i++) {
            if(n[i] == best && can_visit[i]) {
                new_valid_states.push_back(i);
            }
        }

        ans.push_back(best);
        valid_states = new_valid_states;
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
