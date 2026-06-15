#include <bits/stdc++.h>
using namespace std;

int main(){
    int N;
    if(!(cin>>N)) return 0;
    while(N--){
        long long M;
        cin>>M;
        long long d = M-1;
        bool yes=false;
        if(d>=1){
            // solve n(n+1)/2 = d, n>=1
            long long disc = 1+8*d;
            long long s = (long long)sqrtl((long double)disc);
            while(s*s>disc) s--;
            while((s+1)*(s+1)<=disc) s++;
            if(s*s==disc){
                if((s-1)%2==0){
                    long long n=(s-1)/2;
                    if(n>=1) yes=true;
                }
            }
        }
        cout<<(yes?"YES":"NO")<<"\n";
    }
    return 0;
}
