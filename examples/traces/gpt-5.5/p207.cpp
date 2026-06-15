#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long M, Y;
    cin >> N >> M >> Y;

    vector<long long> K(N);
    vector<pair<long long, int>> rem;

    long long sumFloors = 0;

    for (int i = 0; i < N; ++i) {
        long long X;
        cin >> X;

        long long value = M * X;
        K[i] = value / Y;
        long long r = value % Y;

        sumFloors += K[i];
        rem.push_back({r, i});
    }

    long long need = M - sumFloors;

    sort(rem.begin(), rem.end(), [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first > b.first;
        return a.second < b.second;
    });

    for (int i = 0; i < need; ++i) {
        K[rem[i].second]++;
    }

    for (int i = 0; i < N; ++i) {
        if (i) cout << ' ';
        cout << K[i];
    }
    cout << '\n';

    return 0;
}
