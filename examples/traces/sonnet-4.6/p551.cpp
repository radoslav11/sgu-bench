#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n, t1, t2;
    cin >> n >> t1 >> t2;
    
    // Find k1: number of solutions Petya writes
    long long k1 = 1;
    while (true) {
        long long total = k1 + (k1 * t1) / t2;
        if (total < n) k1++;
        else break;
    }
    
    // Find k2: number of solutions Vasya writes
    long long k2 = 1;
    while (true) {
        long long total = k2 + (k2 * t2) / t1;
        if (total < n) k2++;
        else break;
    }
    
    cout << k1 + k2 << " " << max(k1 * t1, k2 * t2) << endl;
    return 0;
}
