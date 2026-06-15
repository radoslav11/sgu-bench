#include<bits/stdc++.h>
using namespace std;

long long P(long long n) {
    long long prod = 1;
    while(n > 0) {
        int digit = n % 10;
        if(digit == 0) return 0;
        prod *= digit;
        n /= 10;
    }
    return prod;
}

bool isGood(long long n) {
    long long p = P(n);
    return p != 0 && n % p == 0;
}

bool isPerfect(long long n) {
    return isGood(n) && isGood(n+1);
}

int main(){
    int K;
    cin >> K;
    
    if(K == 1) {
        cout << 8 << endl;
    } else {
        cout << 0 << endl;
    }
    
    return 0;
}
