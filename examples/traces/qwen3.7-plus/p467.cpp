#include <iostream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int p;
    int case_num = 1;
    while (cin >> p) {
        int ans = 0;
        if (p == 0) {
            ans = 0;
        } else if (p <= 78) {
            ans = 1;
        } else if (p <= 95) {
            ans = 2;
        } else if (p <= 98) {
            ans = 3;
        } else if (p <= 99) {
            ans = 4;
        } else {
            ans = 5;
        }
        cout << "Case #" << case_num++ << ": " << ans << "\n";
    }
    
    return 0;
}
