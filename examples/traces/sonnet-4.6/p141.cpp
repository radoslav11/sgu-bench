#include <bits/stdc++.h>
using namespace std;
int main(){
    long long x1,x2,P,K;
    cin>>x1>>x2>>P>>K;
    for(long long a=-80000;a<=80000;a++){
        long long rem=P-a*x1;
        if(rem%x2!=0) continue;
        long long b=rem/x2;
        long long mink=abs(a)+abs(b);
        if(mink>K) continue;
        if((K-mink)%2!=0) continue;
        if((a+b)%2!=0 && K%2!=(a+b)%2) continue;
        // need k1>=|a|, k2>=|b|, k1+k2=K, k1≡a mod2, k2≡b mod2
        // k1=|a|+2t, k2=K-k1>=|b| => t<=(K-|a|-|b|)/2
        long long k1=abs(a), k2=K-k1;
        if(k2<abs(b)||(k2-abs(b))%2!=0){ k1=abs(a)+1; k2=K-k1; }
        if(k2<abs(b)||(k2-abs(b))%2!=0) continue;
        long long P1=(k1+a)/2, N1=(k1-a)/2, P2=(k2+b)/2, N2=(k2-b)/2;
        if(P1<0||N1<0||P2<0||N2<0) continue;
        cout<<"YES\n"<<P1<<" "<<N1<<" "<<P2<<" "<<N2<<"\n";
        return 0;
    }
    cout<<"NO\n";
}
