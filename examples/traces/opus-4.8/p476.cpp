#include <bits/stdc++.h>
using namespace std;

typedef vector<long long> Big; // base 1e9, little endian
const long long BASE = 1000000000LL;

Big fromInt(long long x){
    Big r;
    if(x==0){ r.push_back(0); return r;}
    while(x){ r.push_back(x%BASE); x/=BASE;}
    return r;
}
void trim(Big &a){ while(a.size()>1 && a.back()==0) a.pop_back(); }

Big mulSmall(const Big &a, long long m){
    Big r(a.size());
    long long carry=0;
    for(size_t i=0;i<a.size();i++){
        long long cur = a[i]*m + carry; // a[i]<1e9, m small
        r[i]=cur%BASE;
        carry=cur/BASE;
    }
    while(carry){ r.push_back(carry%BASE); carry/=BASE;}
    trim(r);
    return r;
}
Big divSmall(const Big &a, long long d){
    Big r(a.size());
    long long rem=0;
    for(int i=a.size()-1;i>=0;i--){
        long long cur = rem*BASE + a[i];
        r[i]=cur/d;
        rem=cur%d;
    }
    trim(r);
    return r;
}
Big addBig(const Big &a, const Big &b){
    Big r;
    long long carry=0;
    size_t n=max(a.size(),b.size());
    for(size_t i=0;i<n||carry;i++){
        long long cur=carry;
        if(i<a.size()) cur+=a[i];
        if(i<b.size()) cur+=b[i];
        r.push_back(cur%BASE);
        carry=cur/BASE;
    }
    trim(r);
    return r;
}
// a - b, assume a>=b
Big subBig(const Big &a, const Big &b){
    Big r;
    long long borrow=0;
    for(size_t i=0;i<a.size();i++){
        long long cur=a[i]-borrow-(i<b.size()?b[i]:0);
        if(cur<0){ cur+=BASE; borrow=1;} else borrow=0;
        r.push_back(cur);
    }
    trim(r);
    return r;
}

int main(){
    int N,K;
    scanf("%d %d",&N,&K);
    vector<array<int,3>> tri(K);
    for(int i=0;i<K;i++){
        scanf("%d %d %d",&tri[i][0],&tri[i][1],&tri[i][2]);
    }
    // adjacency: conflict if share a vertex
    vector<int> adj(K,0);
    for(int i=0;i<K;i++)for(int j=i+1;j<K;j++){
        bool conflict=false;
        for(int x=0;x<3;x++)for(int y=0;y<3;y++) if(tri[i][x]==tri[j][y]) conflict=true;
        if(conflict){ adj[i]|=(1<<j); adj[j]|=(1<<i);}
    }
    // count independent sets by size
    int maxj = min(K,N);
    vector<long long> cnt(K+1,0);
    for(int mask=0; mask < (1<<K); mask++){
        bool ok=true;
        for(int i=0;i<K && ok;i++) if(mask&(1<<i)){
            if(adj[i]&mask){ ok=false;}
        }
        if(ok){
            int pc=__builtin_popcount(mask);
            cnt[pc]++;
        }
    }
    // f[m] = (3m)!/(m! 6^m)
    vector<Big> f(N+1);
    f[0]=fromInt(1);
    for(int m=1;m<=N;m++){
        Big cur=f[m-1];
        cur=mulSmall(cur,(long long)(3*m));
        cur=mulSmall(cur,(long long)(3*m-1));
        cur=mulSmall(cur,(long long)(3*m-2));
        cur=divSmall(cur,6);
        cur=divSmall(cur,m);
        f[m]=cur;
    }
    Big posSum=fromInt(0), negSum=fromInt(0);
    for(int j=0;j<=K;j++){
        if(j>N) break;
        if(cnt[j]==0) continue;
        Big term=mulSmall(f[N-j], cnt[j]);
        if(j%2==0) posSum=addBig(posSum,term);
        else negSum=addBig(negSum,term);
    }
    Big ans=subBig(posSum,negSum);
    // print
    printf("%lld",ans.back());
    for(int i=(int)ans.size()-2;i>=0;i--) printf("%09lld",ans[i]);
    printf("\n");
    return 0;
}
