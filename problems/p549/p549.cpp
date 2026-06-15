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

int n, k;
vector<pair<int, int>> dumbbells;

void read() {
    cin >> n >> k;
    dumbbells.resize(n);
    cin >> dumbbells;
}

void solve() {
    // Each set fixes a multiset of k distinct masses shared by every set, so a
    // mass can contribute one dumbbell to each of the t sets only if it has at
    // least t dumbbells. Group costs by mass; the number of sets t is limited
    // to k (k distinct masses per set) and to how many masses are frequent
    // enough, so the best t is the count of the k-th most frequent mass: with
    // that t we have at least k masses each owning >= t dumbbells.
    //
    // For each eligible mass (count >= t) take its t most expensive dumbbells
    // and sum them; we then pick the k masses with the largest such sums to
    // maximize total cost. If there are fewer than k distinct masses no set
    // can be formed and the answer is 0 0.

    map<int, vector<int>> mass_to_costs;
    for(auto [m, c]: dumbbells) {
        mass_to_costs[m].push_back(c);
    }

    vector<pair<int, int>> frequency;
    for(auto& [mass, costs]: mass_to_costs) {
        sort(costs.rbegin(), costs.rend());
        frequency.push_back({(int)costs.size(), mass});
    }

    sort(frequency.rbegin(), frequency.rend());

    if((int)frequency.size() < k) {
        cout << 0 << ' ' << 0 << '\n';
        return;
    }

    int t = frequency[k - 1].first;

    vector<int64_t> selected;
    for(auto [count, mass]: frequency) {
        if(count < t) {
            break;
        }

        int64_t sum_costs = 0;
        for(int i = 0; i < t; i++) {
            sum_costs += mass_to_costs[mass][i];
        }

        selected.push_back(sum_costs);
    }

    sort(selected.rbegin(), selected.rend());

    int64_t total = 0;
    for(int i = 0; i < k; i++) {
        total += selected[i];
    }

    cout << t << ' ' << total << '\n';
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
