#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll gcd_ext(ll a,ll b,ll&x,ll&y){if(!b){x=1;y=0;return a;}ll x1,y1;ll g=gcd_ext(b,a%b,x1,y1);x=y1;y=x1-(a/b)*y1;return g;}
ll floordiv(ll a,ll b){return a/b-(a%b!=0&&(a^b)<0);}
ll ceildiv(ll a,ll b){return a/b+(a%b!=0&&(a^b)>0);}
int main(){
    ll a,b,c,x1,x2,y1,y2;
    cin>>a>>b>>c>>x1>>x2>>y1>>y2;
    if(a==0&&b==0){if(c==0)cout<<(x2-x1+1)*(y2-y1+1);else cout<<0;return 0;}
    if(a==0){if((-c)%b==0){ll y0=-c/b;if(y0>=y1&&y0<=y2)cout<<x2-x1+1;else cout<<0;}else cout<<0;return 0;}
    if(b==0){if((-c)%a==0){ll x0=-c/a;if(x0>=x1&&x0<=x2)cout<<y2-y1+1;else cout<<0;}else cout<<0;return 0;}
    ll px,py;ll g=gcd_ext(a,b,px,py);
    if((-c)%g!=0){cout<<0;return 0;}
    ll rhs=-c/g,A=a/g,B=b/g;
    px*=rhs;py*=rhs;
    ll tlo=LLONG_MIN/2,thi=LLONG_MAX/2;
    ll lo,hi;
    if(B>0){lo=ceildiv(x1-px,B);hi=floordiv(x2-px,B);}
    else{lo=ceildiv(x2-px,B);hi=floordiv(x1-px,B);}
    tlo=max(tlo,lo);thi=min(thi,hi);
    if(A>0){lo=ceildiv(py-y2,A);hi=floordiv(py-y1,A);}
    else{lo=ceildiv(py-y1,A);hi=floordiv(py-y2,A);}
    tlo=max(tlo,lo);thi=min(thi,hi);
    cout<<max(0LL,thi-tlo+1);
}
