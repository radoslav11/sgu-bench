#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<pair<long long, long long>> intervals(n);
    for (int i = 0; i < n; i++) {
        cin >> intervals[i].first >> intervals[i].second;
    }
    
    // Sort by A ascending
    sort(intervals.begin(), intervals.end());
    
    int redundant = 0;
    long long maxB = LLONG_MIN;
    
    for (int i = 0; i < n; i++) {
        if (maxB > intervals[i].second) {
            redundant++;
        }
        maxB = max(maxB, intervals[i].second);
    }
    
    cout << redundant << endl;
    
    return 0;
}
