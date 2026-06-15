#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<pair<long long, long long>> intervals(N);
    for (int i = 0; i < N; ++i) {
        cin >> intervals[i].first >> intervals[i].second;
    }

    sort(intervals.begin(), intervals.end());

    int redundant = 0;
    long long maxB = -1;

    for (const auto& [A, B] : intervals) {
        if (maxB > B) {
            ++redundant;
        }
        maxB = max(maxB, B);
    }

    cout << redundant << '\n';

    return 0;
}
