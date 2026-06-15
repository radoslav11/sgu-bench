#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, k;
    cin >> n >> k;

    map<int, vector<int>> groups;

    for (int i = 0; i < n; i++) {
        int m, c;
        cin >> m >> c;
        groups[m].push_back(c);
    }

    // Sort each group's costs in descending order
    for (auto& p : groups) {
        sort(p.second.begin(), p.second.end(), greater<int>());
    }

    // If there are no dumbbells, but n>=1 so skip, but groups.size() might be 0? n>=1 so no.
    // Precompute frequency count for frequencies
    vector<int> freqCount(n + 1, 0); // indices 1..n
    for (const auto& p : groups) {
        int s = p.second.size();
        if (s <= n) {
            freqCount[s]++;
        }
    }

    vector<int> countForT(n + 2, 0);
    countForT[0] = groups.size();
    for (int t = 1; t <= n; t++) {
        countForT[t] = countForT[t - 1] - (t - 1 <= n ? freqCount[t - 1] : 0);
    }

    int t_star = 0;
    for (int t = 1; t <= n; t++) {
        if (countForT[t] >= k) {
            t_star = t;
        } else {
            break;
        }
    }

    if (t_star == 0) {
        cout << "0 0\n";
    } else {
        vector<long long> contributions;
        for (const auto& p : groups) {
            if (p.second.size() >= t_star) {
                long long sum = 0;
                for (int i = 0; i < t_star; i++) {
                    sum += p.second[i];
                }
                contributions.push_back(sum);
            }
        }

        // Sort in descending order
        sort(contributions.rbegin(), contributions.rend());
        long long total_cost = 0;
        for (int i = 0; i < k; i++) {
            total_cost += contributions[i];
        }
        cout << t_star << " " << total_cost << "\n";
    }

    return 0;
}
