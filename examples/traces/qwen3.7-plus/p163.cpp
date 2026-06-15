#include <iostream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (cin >> n) {
        int p;
        cin >> p;
        
        long long max_sum = 0;
        for (int i = 0; i < n; ++i) {
            long long x;
            cin >> x;
            
            long long val = 1;
            if (x == 0) {
                val = 0;
            } else {
                for (int j = 0; j < p; ++j) {
                    val *= x;
                }
            }
            
            if (val > 0) {
                max_sum += val;
            }
        }
        
        cout << max_sum << "\n";
    }
    
    return 0;
}
