#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    unordered_map<int, vector<int>> mass2costs;
    mass2costs.reserve(n);
    for (int i = 0; i < n; i++) {
        int m, c;
        cin >> m >> c;
        mass2costs[m].push_back(c);
    }

    vector<pair<int, int>> freq;
    freq.reserve(mass2costs.size());
    for (auto &entry : mass2costs) {
        int mass = entry.first;
        auto &costs = entry.second;
        sort(costs.begin(), costs.end(), greater<int>());
        freq.emplace_back((int)costs.size(), mass);
    }

    sort(freq.begin(), freq.end(), greater<pair<int, int>>());

    if ((int)freq.size() < k) {
        cout << 0 << " " << 0 << "\n";
        return 0;
    }

    int t = freq[k - 1].first;

    vector<ll> sums;
    sums.reserve(freq.size());
    for (auto &p : freq) {
        int count = p.first;
        int mass = p.second;
        if (count < t) {
            break;
        }
        ll s = 0;
        auto &costs = mass2costs[mass];
        for (int i = 0; i < t; i++) {
            s += costs[i];
        }
        sums.push_back(s);
    }

    sort(sums.begin(), sums.end(), greater<ll>());

    ll totalCost = 0;
    for (int i = 0; i < k; i++) {
        totalCost += sums[i];
    }

    cout << t << " " << totalCost << "\n";
    return 0;
}
