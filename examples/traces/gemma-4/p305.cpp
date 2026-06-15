#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

/**
 * Problem Analysis:
 * We have N arithmetic progressions. Each progression i is defined by Ai * K + Bi for K >= 0.
 * We need to assign each progression a unique number from {1, 2, ..., M} such that 
 * we maximize the number of progressions assigned a number belonging to them.
 * This is a maximum bipartite matching problem where one set of nodes is the N progressions
 * and the other is the M possible values. Since M is large (up to 10^9), we can't
 * create a node for each value. However, since N is small (up to 300), we only care 
 * about at most N values that are currently matched.
 * 
 * Algorithm:
 * 1. For each progression, calculate the range of valid K values.
 * 2. Use an augmenting path algorithm (DFS) to find the maximum bipartite matching.
 * 3. For each progression i, we attempt to find an unmatched value in its progression 
 *    or re-match an already matched value by looking for an augmenting path.
 * 4. After maximizing matching, any progression not matched is assigned any 
 *    unused value from {1, ..., M}.
 */

typedef long long ll;

struct Progression {
    ll a, b;
    ll k_min, k_max;
};

int N;
ll M;
vector<Progression> progs;
map<ll, int> match_x_to_i;
vector<ll> match_i_to_x;
vector<bool> is_matched;
vector<bool> visited;
set<ll> M_matched;

bool is_in_Si(int i, ll x) {
    if (progs[i].k_min > progs[i].k_max) return false;
    if (progs[i].a == 0) {
        return x == progs[i].b;
    }
    // A * K + B = x  =>  A * K = x - B
    if ((x - progs[i].b) % progs[i].a == 0) {
        ll k = (x - progs[i].b) / progs[i].a;
        return (k >= progs[i].k_min && k <= progs[i].k_max);
    }
    return false;
}

bool dfs(int i) {
    visited[i] = true;

    // 1. Try to find an unmatched value in Si
    // There are at most N matched values, so we only need to check at most N+1 values of K.
    ll k_start = progs[i].k_min;
    ll k_end = min(progs[i].k_max, k_start + N);
    for (ll k = k_start; k <= k_end; ++k) {
        ll x = progs[i].a * k + progs[i].b;
        if (M_matched.find(x) == M_matched.end()) {
            match_i_to_x[i] = x;
            match_x_to_i[x] = i;
            M_matched.insert(x);
            is_matched[i] = true;
            return true;
        }
    }

    // 2. Try to find an augmenting path through a matched value in Si
    vector<ll> candidates;
    for (auto const& [x, j] : match_x_to_i) {
        if (is_in_Si(i, x)) {
            candidates.push_back(x);
        }
    }

    for (ll x : candidates) {
        int j = match_x_to_i[x];
        if (!visited[j] && dfs(j)) {
            match_i_to_x[i] = x;
            match_x_to_i[x] = i;
            is_matched[i] = true;
            return true;
        }
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> N >> M)) return 0;

    progs.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> progs[i].a >> progs[i].b;
        ll a = progs[i].a;
        ll b = progs[i].b;

        if (a == 0) {
            if (b >= 1 && b <= M) {
                progs[i].k_min = 0;
                progs[i].k_max = 0;
            } else {
                progs[i].k_min = 0;
                progs[i].k_max = -1;
            }
        } else if (a > 0) {
            // 1 <= a*K + b <= M  =>  1-b <= a*K <= M-b
            ll low = 1 - b;
            ll high = M - b;
            if (high < 0) {
                progs[i].k_min = 0;
                progs[i].k_max = -1;
            } else {
                progs[i].k_min = (low <= 0) ? 0 : (low + a - 1) / a;
                progs[i].k_max = high / a;
            }
        } else { // a < 0
            // 1 <= a*K + b <= M  =>  1-b <= a*K <= M-b  =>  b-M <= -a*K <= b-1
            // Let a' = -a > 0. Then b-M <= a'*K <= b-1
            ll a_prime = -a;
            ll low = b - M;
            ll high = b - 1;
            if (high < 0) {
                progs[i].k_min = 0;
                progs[i].k_max = -1;
            } else {
                progs[i].k_min = (low <= 0) ? 0 : (low + a_prime - 1) / a_prime;
                progs[i].k_max = high / a_prime;
            }
        }
    }

    match_i_to_x.assign(N, 0);
    is_matched.assign(N, false);
    match_x_to_i.clear();
    M_matched.clear();

    for (int i = 0; i < N; ++i) {
        visited.assign(N, false);
        dfs(i);
    }

    set<ll> used_x = M_matched;
    ll current_x = 1;
    for (int i = 0; i < N; ++i) {
        if (is_matched[i]) {
            cout << match_i_to_x[i] << (i == N - 1 ? "" : " ");
        } else {
            while (used_x.count(current_x)) current_x++;
            cout << current_x << (i == N - 1 ? "" : " ");
            used_x.insert(current_x);
        }
    }
    cout << endl;

    return 0;
}

