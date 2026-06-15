#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long A, alpha, beta, gamma, M, k;
    cin >> A >> alpha >> beta >> gamma >> M >> k;

    vector<long long> seq;
    unordered_map<long long, long long> first;

    auto next_value = [&](long long x) -> long long {
        return (alpha * x * x + beta * x + gamma) % M;
    };

    long long x = A;

    while (!first.count(x)) {
        first[x] = (long long)seq.size();
        seq.push_back(x);

        if ((long long)seq.size() - 1 == k) {
            cout << x << '\n';
            return 0;
        }

        x = next_value(x);
    }

    long long cycle_start = first[x];
    long long cycle_len = (long long)seq.size() - cycle_start;

    long long ans;
    if (k < (long long)seq.size()) {
        ans = seq[k];
    } else {
        ans = seq[cycle_start + (k - cycle_start) % cycle_len];
    }

    cout << ans << '\n';
    return 0;
}
