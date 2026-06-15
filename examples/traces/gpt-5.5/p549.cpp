#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    const int MAXM = 4000;
    vector<vector<int>> byMass(MAXM + 1);

    for (int i = 0; i < n; ++i) {
        int m, c;
        cin >> m >> c;
        byMass[m].push_back(c);
    }

    vector<int> counts;
    for (int m = 1; m <= MAXM; ++m) {
        if (!byMass[m].empty()) {
            counts.push_back((int)byMass[m].size());
        }
    }

    if ((int)counts.size() < k) {
        cout << "0 0\n";
        return 0;
    }

    sort(counts.rbegin(), counts.rend());
    int t = counts[k - 1];

    vector<long long> sums;

    for (int m = 1; m <= MAXM; ++m) {
        if ((int)byMass[m].size() >= t) {
            sort(byMass[m].rbegin(), byMass[m].rend());

            long long sum = 0;
            for (int i = 0; i < t; ++i) {
                sum += byMass[m][i];
            }

            sums.push_back(sum);
        }
    }

    sort(sums.rbegin(), sums.rend());

    long long answerCost = 0;
    for (int i = 0; i < k; ++i) {
        answerCost += sums[i];
    }

    cout << t << ' ' << answerCost << '\n';

    return 0;
}
