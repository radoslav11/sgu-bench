#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;

ull sumDigits(ull n){
    if(n==0) return 0;
    ull total=0;
    ull p=1;
    while(true){
        ull higher = n/(p*10);
        ull cur = (n/p)%10;
        ull lower = n%p;
        total += higher*45ull*p;
        total += (cur*(cur-1)/2)*p;
        total += cur*(lower+1);
        if(p > n/10) break;
        p*=10;
    }
    return total;
}

int main(){
    ull l,r,k;
    cin>>l>>r>>k;
    ull s = sumDigits(r) - sumDigits(l-1);
    cout << (s/k) << "\n";
    return 0;
}
