#include <iostream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (cin >> n) {
        if (n == 1) {
            cout << "1 0\n";
        } else if (n == 2) {
            cout << "2 1\n1 2\n";
        } else if (n == 3) {
            cout << "3 3\n1 2\n2 3\n1 3\n";
        } else {
            cout << n << " " << 2 * n - 4 << "\n";
            for (int i = 3; i <= n; ++i) {
                cout << 1 << " " << i << "\n";
                cout << 2 << " " << i << "\n";
            }
        }
    }
    
    return 0;
}
