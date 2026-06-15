#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll extgcd(ll a, ll b, ll &x, ll &y){
    if(b==0){ x=1; y=0; return a; }
    ll x1,y1; ll g=extgcd(b,a%b,x1,y1);
    x=y1; y=x1-(a/b)*y1; return g;
}

ll fdiv(ll a, ll b){
    ll q=a/b, r=a%b;
    if(r!=0 && (r<0)) q--;
    return q;
}
ll cdiv(ll a, ll b){
    ll q=a/b, r=a%b;
    if(r!=0 && (r>0)) q++;
    return q;
}

int main(){
    ll a,b,c,x1,x2,y1,y2;
    cin>>a>>b>>c>>x1>>x2>>y1>>y2;
    if(x1>x2 || y1>y2){ cout<<0<<endl; return 0; }
    
    if(a==0 && b==0){
        if(c==0){
            ll cnt=(x2-x1+1)*(y2-y1+1);
            cout<<cnt<<endl;
        } else cout<<0<<endl;
        return 0;
    }
    if(a==0){
        if((-c)%b!=0){ cout<<0<<endl; return 0; }
        ll y=(-c)/b;
        if(y>=y1 && y<=y2) cout<<(x2-x1+1)<<endl;
        else cout<<0<<endl;
        return 0;
    }
    if(b==0){
        if((-c)%a!=0){ cout<<0<<endl; return 0; }
        ll x=(-c)/a;
        if(x>=x1 && x<=x2) cout<<(y2-y1+1)<<endl;
        else cout<<0<<endl;
        return 0;
    }
    
    ll rhs=-c;
    ll x0,y0;
    ll g=extgcd(a,b,x0,y0);
    if(rhs%g!=0){ cout<<0<<endl; return 0; }
    ll mul=rhs/g;
    ll px=x0*mul;
    ll py=y0*mul;
    ll dx=b/g;
    ll dy=-a/g;
    
    ll tlo=LLONG_MIN, thi=LLONG_MAX;
    {
        if(dx>0){
            ll lo=cdiv(x1-px,dx);
            ll hi=fdiv(x2-px,dx);
            tlo=max(tlo,lo); thi=min(thi,hi);
        } else {
            ll d=-dx;
            ll lo=cdiv(px-x2,d);
            ll hi=fdiv(px-x1,d);
            tlo=max(tlo,lo); thi=min(thi,hi);
        }
    }
    {
        if(dy>0){
            ll lo=cdiv(y1-py,dy);
            ll hi=fdiv(y2-py,dy);
            tlo=max(tlo,lo); thi=min(thi,hi);
        } else {
            ll d=-dy;
            ll lo=cdiv(py-y2,d);
            ll hi=fdiv(py-y1,d);
            tlo=max(tlo,lo); thi=min(thi,hi);
        }
    }
    
    if(thi<tlo) cout<<0<<endl;
    else cout<<(thi-tlo+1)<<endl;
    return 0;
}
