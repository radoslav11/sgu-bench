#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void solve() {
    int n;
    if (!(cin >> n)) return;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    
    int max_hill = 0;
    int max_dale = 0;
    
    for (int j = 1; j < n - 1; j++) {
        // Check if j is a peak (potential hill)
        if (a[j - 1] < a[j] && a[j] > a[j + 1]) {
            int i = j;
            while (i > 0 && a[i - 1] < a[i]) {
                i--;
            }
            max_hill = max(max_hill, a[j] - a[i]);
        }
        
        // Check if j is a valley (potential dale)
        if (a[j - 1] > a[j] && a[j] < a[j + 1]) {
            int i = j;
            while (i > 0 && a[i - 1] > a[i]) {
                i--;
            }
            max_dale = max(max_dale, a[i] - a[j]);
        }
    }
    
    cout << max_hill << " " << max_dale << "\n";
}

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}
