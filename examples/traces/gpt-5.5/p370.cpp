#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long N, M;
    cin >> N >> M;

    int A = (int)(M - 1);
    int B = (int)(N - 1);
    int L = min(A, B);

    vector<int> mu(L + 1), primes;
    vector<bool> isComposite(L + 1);
    if (L >= 1) mu[1] = 1;

    for (int i = 2; i <= L; ++i) {
        if (!isComposite[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes) {
            long long v = 1LL * i * p;
            if (v > L) break;
            isComposite[v] = true;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }

    long long answer = 0;

    if (A > 0) ++answer; // horizontal direction
    if (B > 0) ++answer; // vertical direction

    for (int d = 1; d <= L; ++d) {
        answer += 1LL * mu[d] * (A / d) * (B / d);
    }

    cout << answer << '\n';

    return 0;
}
