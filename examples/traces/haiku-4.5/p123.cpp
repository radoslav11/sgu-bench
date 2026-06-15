#include <iostream>
using namespace std;

int main() {
    int k;
    cin >> k;
    
    long long sum = 0;
    long long f1 = 1, f2 = 1;
    
    if (k >= 1) {
        sum += f1;  // F1 = 1
    }
    if (k >= 2) {
        sum += f2;  // F2 = 1
    }
    
    // Generate remaining Fibonacci numbers
    for (int i = 3; i <= k; i++) {
        long long next = f1 + f2;
        sum += next;
        f1 = f2;
        f2 = next;
    }
    
    cout << sum << endl;
    
    return 0;
}
