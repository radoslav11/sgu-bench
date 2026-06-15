#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int n, m;
    cin >> n >> m;
    
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
    }
    
    for (int i = 0; i < m; i++) {
        int p, q;
        cin >> p >> q;
        // Convert to 0-indexed: [p-1, q-1]
        reverse(a.begin() + p - 1, a.begin() + q);
    }
    
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << " ";
        cout << a[i];
    }
    cout << endl;
    
    return 0;
}
