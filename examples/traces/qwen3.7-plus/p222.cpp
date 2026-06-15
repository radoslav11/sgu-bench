#include <iostream>

using namespace std;

long long factorial(int n) {
    long long res = 1;
    for (int i = 2; i <= n; ++i) {
        res *= i;
    }
    return res;
}

long long nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return factorial(n) / (factorial(r) * factorial(n - r));
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, k;
    if (cin >> n >> k) {
        if (k > n) {
            cout << 0 << "\n";
        } else {
            long long ways = nCr(n, k) * nCr(n, k) * factorial(k);
            cout << ways << "\n";
        }
    }
    return 0;
}
