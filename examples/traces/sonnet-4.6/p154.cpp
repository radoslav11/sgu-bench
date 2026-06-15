#include<bits/stdc++.h>
using namespace std;

long long trailingZeros(long long n) {
    long long count = 0;
    long long p = 5;
    while (p <= n) {
        count += n / p;
        p *= 5;
    }
    return count;
}

int main(){
    long long Q;
    cin >> Q;
    
    if (Q == 0) {
        cout << 0 << endl;
        return 0;
    }
    
    // Binary search for smallest N with trailingZeros(N) >= Q
    long long lo = 0, hi = 5LL * Q + 5;
    while (lo < hi) {
        long long mid = (lo + hi) / 2;
        if (trailingZeros(mid) >= Q) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    
    if (trailingZeros(lo) == Q) {
        cout << lo << endl;
    } else {
        cout << "No solution" << endl;
    }
    
    return 0;
}
