#include<bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main(){
    int n;
    cin >> n;
    
    long long num = 1, den = 1;
    
    for(int k = 1; k <= n; k++) {
        num *= k;
        den *= (2*k + 1);
        
        long long g = gcd(num, den);
        num /= g;
        den /= g;
    }
    
    cout << num << "/" << den << endl;
    
    return 0;
}
