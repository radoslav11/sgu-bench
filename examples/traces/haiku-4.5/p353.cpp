#include <iostream>
using namespace std;

int main() {
    long long N, k1, k2, p1, p2, p3;
    cin >> N >> k1 >> k2 >> p1 >> p2 >> p3;
    
    if (N < p1) {
        cout << 0 << endl;
        return 0;
    }
    
    long long balance = N - p1;
    long long seconds = k1;
    
    // Next k2 seconds at p2 each
    long long i = 0;
    while (i < k2 && balance > 0) {
        balance -= p2;
        seconds++;
        i++;
    }
    
    // Remaining seconds at p3 each
    while (balance > 0) {
        balance -= p3;
        seconds++;
    }
    
    cout << seconds << endl;
    
    return 0;
}
