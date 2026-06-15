#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    // k=1: sum must be exactly 21
    if (n == 21) {
        cout << 1 << endl;
        return 0;
    }
    
    // For k >= 2: n in [9k+12, 19k+2]
    // minimum k >= ceil((n-2)/19)
    // need 9k+12 <= n => k <= (n-12)/9
    
    // find min k >= 2 such that 9k+12 <= n <= 19k+2
    if (n < 30) { // min for k=2 is 30
        cout << -1 << endl;
        return 0;
    }
    
    int k = max(2, (int)ceil((n - 2.0) / 19.0));
    
    if (9*k + 12 <= n && n <= 19*k + 2) {
        cout << k << endl;
    } else {
        cout << -1 << endl;
    }
    
    return 0;
}
