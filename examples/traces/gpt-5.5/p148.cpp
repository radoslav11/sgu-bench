#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n;
    vector<long long> bit;

    Fenwick(int n = 0) : n(n), bit(n + 1, 0) {}

    void add(int idx, long long val) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += val;
    }

    long long sum(int idx) const {
        long long res = 0;
        for (; idx > 0; idx -= idx & -idx) res += bit[idx];
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<long long> W(N + 1), L(N + 1), P(N + 1);
    vector<long long> S(N + 1, 0), A(N + 1);

    for (int i = 1; i <= N; ++i) {
        cin >> W[i] >> L[i] >> P[i];
        S[i] = S[i - 1] + W[i];
        A[i] = S[i] - L[i];
    }

    vector<long long> vals;
    vals.reserve(N);
    for (int i = 1; i <= N; ++i) vals.push_back(A[i]);

    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());

    Fenwick fw((int)vals.size());

    long long bestCost = LLONG_MAX;
    int bestT = 0;

    for (int t = N - 1; t >= 0; --t) {
        int addIdx = t + 1;
        int posAdd = lower_bound(vals.begin(), vals.end(), A[addIdx]) - vals.begin() + 1;
        fw.add(posAdd, P[addIdx]);

        int posQuery = upper_bound(vals.begin(), vals.end(), S[t]) - vals.begin();
        long long cost = fw.sum(posQuery);

        if (cost < bestCost) {
            bestCost = cost;
            bestT = t;
        }
    }

    for (int i = bestT + 1; i <= N; ++i) {
        if (A[i] <= S[bestT]) {
            cout << i << '\n';
        }
    }

    return 0;
}
