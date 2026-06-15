#include <iostream>
#include <vector>
using namespace std;

int main() {
    long long n, m;
    cin >> n >> m;
    
    if (m == 0) {
        cout << 1 << endl;
        return 0;
    }
    
    // B[0] = 0, B[1] = 1
    // For m >= 1, we need B[m-1] to compute answer = n * B[m-1]
    if (m == 1) {
        cout << 0 << endl;
        return 0;
    }
    
    long long prev2 = 0; // B[0]
    long long prev1 = 1; // B[1]
    long long current = 0;
    
    // Compute B[2] to B[m-1]
    for (int k = 2; k <= m-1; k++) {
        current = 2 * prev1 + n * prev2;
        prev2 = prev1;
        prev1 = current;
    }
    
    cout << n * prev1 << endl;
    
    return 0;
}
