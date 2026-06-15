#include <bits/stdc++.h>
using namespace std;
int main(){
    long long n;
    cin>>n;
    long long p1,q1,p2,q2,p3,q3;
    cin>>p1>>q1>>p2>>q2>>p3>>q3;
    auto cheb=[](long long ax,long long ay,long long bx,long long by){
        return max(llabs(ax-bx),llabs(ay-by));
    };
    long long D=cheb(p1,q1,p2,q2);
    long long meet=D-1;
    long long db=cheb(p3,q3,p1,q1);
    long long dw=cheb(p3,q3,p2,q2);
    long long blackMoves=meet/2;
    long long bwMoves=blackMoves;
    bool canW=(dw-1)<=bwMoves;
    bool canB=(db-1)<=bwMoves;
    if(canW||canB){
        long long ans=LLONG_MAX;
        if(canW) ans=min(ans,max(1LL,dw-1));
        if(canB) ans=min(ans,max(1LL,db-1));
        cout<<"YES\n"<<ans<<"\n";
    } else {
        cout<<"NO\n"<<meet<<"\n";
    }
    return 0;
}
