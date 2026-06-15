#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll mulmod(ll a, ll b, ll m){ return (__int128)a*b%m; }
ll powmod(ll a, ll b, ll m){ ll r=1%m; a%=m; while(b){ if(b&1) r=mulmod(r,a,m); a=mulmod(a,a,m); b>>=1;} return r; }

vector<ll> factorize(ll n){ vector<ll> f; for(ll i=2;i*i<=n;i++){ if(n%i==0){ f.push_back(i); while(n%i==0)n/=i;}} if(n>1)f.push_back(n); return f; }

ll ext_gcd(ll a, ll b, ll&x, ll&y){ if(b==0){x=1;y=0;return a;} ll x1,y1; ll g=ext_gcd(b,a%b,x1,y1); x=y1; y=x1-(a/b)*y1; return g;}
ll inv(ll a, ll m){ ll x,y; ext_gcd((a%m+m)%m,m,x,y); return (x%m+m)%m; }

ll primitive_root(ll p){
    if(p==2) return 1;
    ll phi=p-1; auto f=factorize(phi);
    for(ll g=2;g<p;g++){
        bool ok=true;
        for(ll q:f){ if(powmod(g,phi/q,p)==1){ok=false;break;} }
        if(ok) return g;
    }
    return -1;
}

ll bsgs(ll g, ll h, ll p){
    ll n=(ll)ceil(sqrt((double)(p-1)))+1;
    unordered_map<ll,ll> mp; mp.reserve(n*2);
    ll cur=1%p;
    for(ll j=0;j<n;j++){ if(mp.find(cur)==mp.end()) mp[cur]=j; cur=mulmod(cur,g,p);}
    ll gn=powmod(g,n,p);
    ll ginvn=inv(gn,p);
    cur=h%p;
    for(ll i=0;i<=n;i++){
        auto it=mp.find(cur);
        if(it!=mp.end()) return i*n+it->second;
        cur=mulmod(cur,ginvn,p);
    }
    return -1;
}

int main(){
    ll P,K,A;
    cin>>P>>K>>A;
    if(A%P==0){ cout<<1<<"\n"<<0<<"\n"; return 0; }
    ll g=primitive_root(P);
    ll a=bsgs(g,A%P,P);
    ll m=P-1;
    ll d=__gcd(K%m, m);
    if(a%d!=0){ cout<<0<<"\n\n"; return 0; }
    ll md=m/d;
    ll y0=mulmod((a/d)%md, inv((K/d)%md, md), md);
    set<ll> roots;
    ll gy0=powmod(g,y0,P);
    ll step=powmod(g,md,P);
    ll cur=gy0;
    for(ll t=0;t<d;t++){ roots.insert(cur); cur=mulmod(cur,step,P); }
    cout<<roots.size()<<"\n";
    bool first=true;
    for(ll r:roots){ if(!first)cout<<" "; cout<<r; first=false; }
    cout<<"\n";
    return 0;
}
