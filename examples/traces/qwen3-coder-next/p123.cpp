#include <iostream>
using namespace std;

int main() {
    int K;
    cin >> K;
    
    if (K == 1) {
        cout << 1 << endl;
        return 0;
    }
    
    long long a = 1, b = 1; // F1 and F2
    long long sum = a + b;
    
    if (K == 2) {
        cout << sum << endl;
        return 0;
    }
    
    for (int i = 3; i <= K; i++) {
        long long c = a + b;
        sum += c;
        a = b;
        b = c;
    }
    
    cout << sum << endl;
    return 0;
}
