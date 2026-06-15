#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    const int MAXV = 1000001;
    vector<bool> exists(MAXV, false);
    
    int maxVal = 0;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        if (x < MAXV) {
            exists[x] = true;
        }
        maxVal = max(maxVal, x);
    }
    
    // For each d from maxVal down to 1, count how many multiples exist
    for (int d = maxVal; d >= 1; d--) {
        int count = 0;
        for (int multiple = d; multiple <= maxVal && multiple < MAXV; multiple += d) {
            if (exists[multiple]) {
                count++;
                if (count >= 2) {
                    cout << d << endl;
                    return 0;
                }
            }
        }
    }
    
    cout << 1 << endl;
    return 0;
}
