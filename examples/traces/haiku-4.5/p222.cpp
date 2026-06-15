#include <iostream>
using namespace std;

long long factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

long long binomial(int n, int k) {
    if (k > n || k < 0) return 0;
    if (k == 0 || k == n) return 1;
    
    long long result = 1;
    for (int i = 0; i < k; i++) {
        result *= (n - i);
        result /= (i + 1);
    }
    return result;
}

int main() {
    int n, k;
    cin >> n >> k;
    
    if (k > n) {
        cout << 0 << endl;
        return 0;
    }
    
    long long ways = binomial(n, k) * binomial(n, k) * factorial(k);
    cout << ways << endl;
    
    return 0;
}
