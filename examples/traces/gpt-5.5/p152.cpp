#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<long long> A(N);
    long long sumVotes = 0;

    for (int i = 0; i < N; ++i) {
        cin >> A[i];
        sumVotes += A[i];
    }

    if (sumVotes == 0) {
        cout << "No solution\n";
        return 0;
    }

    vector<int> ans(N);
    vector<int> fractional;
    int sumParts = 0;

    for (int i = 0; i < N; ++i) {
        long long value = A[i] * 100;
        ans[i] = value / sumVotes;
        sumParts += ans[i];

        if (value % sumVotes != 0) {
            fractional.push_back(i);
        }
    }

    int need = 100 - sumParts;

    if (need < 0 || need > (int)fractional.size()) {
        cout << "No solution\n";
        return 0;
    }

    for (int i = 0; i < need; ++i) {
        ans[fractional[i]]++;
    }

    for (int i = 0; i < N; ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';

    return 0;
}
