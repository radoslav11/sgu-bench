#include <iostream>
#include <vector>
using namespace std;

long long factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

long long comb(int n, int k) {
    if (k > n || k < 0) return 0;
    if (k == 0 || k == n) return 1;
    return factorial(n) / (factorial(k) * factorial(n - k));
}

int main() {
    int n, k;
    cin >> n >> k;
    
    if (k > n) {
        cout << 0 << endl;
    } else {
        long long c = comb(n, k);
        long long result = c * c * factorial(k);
        cout << result << endl;
    }
    
    return 0;
}
