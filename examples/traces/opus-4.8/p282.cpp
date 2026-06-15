#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<uint32_t> BN; const ull BASE=1000000000ULL;
int N,M,P;
BN fromull(ull x){BN r; if(x==0){r.push_back(0);return r;} while(x){r.push_back(x%BASE);x/=BASE;} return r;}
void mulsmall(BN&a,ull m){ull carry=0;for(size_t i=0;i<a.size();i++){__uint128_t cur=(__uint128_t)a[i]*m+carry;a[i]=cur%BASE;carry=cur/BASE;}while(carry){a.push_back(carry%BASE);carry/=BASE;}}
int divsmall(BN&a,ull m){ull rem=0;for(int i=(int)a.size()-1;i>=0;i--){__uint128_t cur=(__uint128_t)rem*BASE+a[i];a[i]=cur/m;rem=cur%m;}while(a.size()>1&&a.back()==0)a.pop_back();return rem;}
BN S; int cnt[60];
ll powmod(ll b,ll e,ll m){b%=m;ll r=1%m;while(e){if(e&1)r=r*b%m;b=b*b%m;e>>=1;}return r;}
ll gcd_(ll a,ll b){while(b){a%=b;swap(a,b);}return a;}
void rec(int rem,int maxp){
    if(rem==0){
        BN c=fromull(1);
        for(int i=2;i<=N;i++) mulsmall(c,i);
        for(int i=1;i<=N;i++) if(cnt[i]){ for(int k=0;k<cnt[i];k++) divsmall(c,i); for(int k=1;k<=cnt[i];k++) divsmall(c,k);}
        ll E=0;
        for(int i=1;i<=N;i++) if(cnt[i]){ E+=(ll)cnt[i]*(i/2); E+=(ll)cnt[i]*(cnt[i]-1)/2*i; }
        for(int i=1;i<=N;i++) if(cnt[i]) for(int j=i+1;j<=N;j++) if(cnt[j]) E+=(ll)cnt[i]*cnt[j]*gcd_(i,j);
        ll mE=powmod(M,E,P);
        mulsmall(c,(ull)mE);
        ull carry=0; for(size_t i=0;i<c.size()||carry;i++){if(S.size()<=i)S.push_back(0);__uint128_t cur=S[i]+carry+(i<c.size()?(__uint128_t)c[i]:0);S[i]=cur%BASE;carry=cur/BASE;}
        return;
    }
    for(int i=min(rem,maxp);i>=1;i--){ cnt[i]++; rec(rem-i,i); cnt[i]--; }
}
int main(){
    cin>>N>>M>>P; S=fromull(0);
    rec(N,N);
    for(int i=2;i<=N;i++) divsmall(S,i);
    ll res=0; ull bp=BASE%P;
    for(int i=(int)S.size()-1;i>=0;i--){ res=(res*bp + S[i]%P)%P;}
    cout<<res<<"\n";
}
