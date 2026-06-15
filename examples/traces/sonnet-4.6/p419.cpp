#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int DX[]={1,0,-1,-1,0,1};
const int DY[]={0,1,1,0,-1,-1};

int main(){
    ll X,Y,d2,d1,b;
    cin>>X>>Y>>d2>>d1>>b;
    
    auto solve2=[](ll A,ll lo,ll hi,ll lo2,ll hi2)->ll{
        // min(a+c) s.t. a-c=A, a in[lo,hi], c in[lo2,hi2]
        // c=a-A; a in[max(lo,lo2+A),min(hi,hi2+A)]
        ll alo=max(lo,lo2+A),ahi=min(hi,hi2+A);
        if(alo>ahi)return(ll)4e18;
        return 2*alo-A; // =alo+(alo-A)
    };
    
    ll ans=4e18;
    
    for(ll k=0;k<=4000;k++){
        ll cnt[6]={};
        for(int d=0;d<6;d++)cnt[d]=k;
        int d=d1;
        while(true){cnt[d]++;if(d==d2)break;d=(d+1)%6;}
        
        ll l[6],u[6];
        for(int i=0;i<6;i++){l[i]=cnt[i];u[i]=cnt[i]*b;}
        
        // lower bound
        ll lb=0;for(int i=0;i<6;i++)lb+=l[i];
        if(lb>=ans)break;
        
        // f(t)=solve2(X+t,l0,u0,l3,u3)+solve2(Y-t,l1,u1,l4,u4)+solve2(t,l2,u2,l5,u5)
        // convex piecewise linear, ternary search
        auto F=[&](ll t)->ll{
            ll ft=solve2(X+t,l[0],u[0],l[3],u[3]);
            ll gt=solve2(Y-t,l[1],u[1],l[4],u[4]);
            ll ht=solve2(t,l[2],u[2],l[5],u[5]);
            if(ft>=(ll)4e17||gt>=(ll)4e17||ht>=(ll)4e17)return(ll)4e18;
            return ft+gt+ht;
        };
        
        ll tlo=l[2]-u[5],thi=u[2]-l[5];
        
        // ternary search on [tlo,thi]
        ll lo=tlo,hi=thi;
        while(hi-lo>2){
            ll m1=lo+(hi-lo)/3,m2=hi-(hi-lo)/3;
            if(F(m1)<F(m2))hi=m2;
            else lo=m1;
        }
        for(ll t=lo;t<=hi;t++)ans=min(ans,F(t));
    }
    cout<<ans<<endl;
}
