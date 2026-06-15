#include <iostream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (cin >> n) {
        if (n < 9) {
            cout << 0 << "\n";
        } else if (n == 9) {
            cout << 8 << "\n";
        } else {
            cout << "72";
            for (int i = 0; i < n - 10; ++i) {
                cout << '0';
            }
            cout << "\n";
        }
    }
    
    return 0;
}
