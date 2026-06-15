#include <bits/stdc++.h>
using namespace std;

static void printHalf(long long x) {
    if (x % 2 == 0) {
        cout << x / 2;
    } else {
        cout << x / 2 << ".5";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int L, M;
    cin >> L >> M;

    vector<int> P(M);
    for (int i = 0; i < M; ++i) cin >> P[i];

    int N;
    cin >> N;

    vector<int> D(N);
    D[0] = 0;
    for (int i = 1; i < N; ++i) cin >> D[i];

    int maxS2 = 2 * (L - D.back());

    vector<int> candidates;
    candidates.reserve(M * (2 * N) + 2);

    candidates.push_back(0);
    candidates.push_back(maxS2);

    for (int p : P) {
        for (int j = 0; j < N; ++j) {
            int s2 = 2 * (p - D[j]);
            if (0 <= s2 && s2 <= maxS2) {
                candidates.push_back(s2);
            }
        }

        for (int j = 0; j + 1 < N; ++j) {
            int s2 = 2 * p - D[j] - D[j + 1];
            if (0 <= s2 && s2 <= maxS2) {
                candidates.push_back(s2);
            }
        }
    }

    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());

    vector<int> D2(N);
    for (int i = 0; i < N; ++i) D2[i] = 2 * D[i];

    long long bestSum2 = -1;
    int bestS2 = 0;

    for (int s2 : candidates) {
        long long sum2 = 0;

        for (int p : P) {
            int target = 2 * p - s2;

            auto it = lower_bound(D2.begin(), D2.end(), target);

            int bestDist = INT_MAX;

            if (it != D2.end()) {
                bestDist = min(bestDist, abs(*it - target));
            }
            if (it != D2.begin()) {
                --it;
                bestDist = min(bestDist, abs(*it - target));
            }

            sum2 += bestDist;
        }

        if (sum2 > bestSum2) {
            bestSum2 = sum2;
            bestS2 = s2;
        }
    }

    printHalf(bestS2);
    cout << ' ';
    printHalf(bestSum2);
    cout << '\n';

    return 0;
}
