#include<bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    if(b == 0) return a;
    return gcd(b, a % b);
}

int main(){
    int caseNum = 1;
    int N;
    while(cin >> N) {
        long long num, den;
        
        if(N == 1) {
            num = -1;
            den = 1;
        } else {
            num = N - 2;
            den = (long long)N * N - 1;
            
            long long g = gcd(abs(num), abs(den));
            num /= g;
            den /= g;
        }
        
        cout << "Case #" << caseNum << ": " << num << "/" << den << "\n";
        caseNum++;
    }
    
    return 0;
}
