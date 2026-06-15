#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> T(n), L(n);
    for (int i = 0; i < n; i++) cin >> T[i];
    for (int i = 0; i < n; i++) cin >> L[i];
    
    // Sort by L descending
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int a, int b) {
        return L[a] > L[b];
    });
    
    int ans = 0;
    int time_so_far = 0;
    for (int i = 0; i < n; i++) {
        int j = idx[i];
        time_so_far += T[j];
        ans = max(ans, time_so_far + L[j]);
    }
    
    cout << ans << endl;
    return 0;
}
