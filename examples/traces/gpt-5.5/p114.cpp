#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<pair<int, int>> cities(N);
    long long total = 0;

    for (int i = 0; i < N; ++i) {
        cin >> cities[i].first >> cities[i].second;
        total += cities[i].second;
    }

    sort(cities.begin(), cities.end());

    long long pref = 0;
    double answer = cities[0].first;

    for (int i = 0; i < N; ++i) {
        pref += cities[i].second;
        if (pref * 2 >= total) {
            answer = cities[i].first;
            break;
        }
    }

    cout << fixed << setprecision(5) << answer << '\n';

    return 0;
}
