#include <iostream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long x;
    if (cin >> x) {
        cout << 2 * x + 1 << "\n";
    }
    
    return 0;
}
