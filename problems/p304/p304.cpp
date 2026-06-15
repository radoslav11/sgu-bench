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

int n, k, p;
vector<int> b;
vector<vector<pair<int, int>>> gums;

void read() {
    cin >> n >> k >> p;
    b.resize(k);
    cin >> b;
    gums = vector<vector<pair<int, int>>>(k);
    for(int i = 0; i < n; ++i) {
        int cost, gum;
        cin >> cost >> gum;
        gums[gum - 1].push_back({cost, i + 1});
    }

    for(auto& gum: gums) {
        sort(gum.begin(), gum.end());
    }
}

void solve() {
    // To cure t teeth as cheaply as possible we choose, per gum, how many of its
    // teeth to treat; within a gum it is always optimal to take the cheapest
    // ones, so each gum's teeth are pre-sorted by cost. Curing any teeth on a
    // gum also pays its anesthesia b[g].
    //
    // dp[g][t] = minimum total cost to cure exactly t teeth using only the first
    // g gums, stored as {cost, count taken from gum g}. Transitions from
    // dp[g][t] either skip gum g (count -1) or take its p + 1 cheapest teeth,
    // paying b[g] plus their prefix-sum of costs.
    //
    // The answer is the largest t whose minimum cost is at most p; we then walk
    // the stored counts back from dp[k][t] to recover which teeth were chosen.

    vector<vector<pair<int, int>>> dp(
        k + 1, vector<pair<int, int>>(n + 1, {INT_MAX, 0})
    );
    dp[0][0] = {0, 0};

    for(int g = 0; g < k; ++g) {
        for(int t = 0; t <= n; ++t) {
            if(dp[g][t].first == INT_MAX) {
                continue;
            }

            int cost = b[g];
            if(dp[g][t].first < dp[g + 1][t].first) {
                dp[g + 1][t] = {dp[g][t].first, -1};
            }

            for(int j = 0; j < min((int)gums[g].size(), n - t); ++j) {
                cost += gums[g][j].first;
                if(dp[g][t].first + cost < dp[g + 1][t + j + 1].first) {
                    dp[g + 1][t + j + 1] = {dp[g][t].first + cost, j + 1};
                }
            }
        }
    }

    int max_teeth = 0;
    while(max_teeth < n && dp[k][max_teeth + 1].first <= p) {
        max_teeth++;
    }

    cout << max_teeth << '\n';

    vector<int> solution;
    int current_teeth = max_teeth;
    for(int g = k; g > 0; --g) {
        int selected = dp[g][current_teeth].second;
        if(selected != -1) {
            for(int i = 0; i < selected; ++i) {
                solution.push_back(gums[g - 1][i].second);
            }

            current_teeth -= selected;
        }
    }

    sort(solution.begin(), solution.end());
    cout << solution << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
