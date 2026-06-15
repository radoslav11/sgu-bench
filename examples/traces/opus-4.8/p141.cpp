#include <bits/stdc++.h>
using namespace std;
long long egcd(long long a,long long b,long long&x,long long&y){
    if(b==0){x=1;y=0;return a;}
    long long x1,y1;long long g=egcd(b,a%b,x1,y1);
    x=y1;y=x1-(a/b)*y1;return g;
}
int main(){
    long long x1,x2,P,K;
    cin>>x1>>x2>>P>>K;
    long long g=__gcd(x1,x2);
    if(P%g!=0){cout<<"NO\n";return 0;}
    long long xx,yy;
    egcd(x1,x2,xx,yy);
    long long mul=P/g;
    long long a0=xx*mul, b0=yy*mul;
    long long da=x2/g, db=x1/g;
    long long t0 = -(long long)llround((double)a0/da);
    for(long long t=t0-100000;t<=t0+100000;t++){
        long long a=a0+da*t;
        long long b=b0-db*t;
        long long need=K-llabs(a)-llabs(b);
        if(need<0) continue;
        if(need%2!=0) continue;
        long long s=need/2;
        long long k1=llabs(a)+2*s;
        long long k2=llabs(b);
        long long P1=(k1+a)/2, N1=(k1-a)/2;
        long long P2=(k2+b)/2, N2=(k2-b)/2;
        cout<<"YES\n";
        cout<<P1<<" "<<N1<<" "<<P2<<" "<<N2<<"\n";
        return 0;
    }
    cout<<"NO\n";
    return 0;
}
