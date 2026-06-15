#include <bits/stdc++.h>
using namespace std;

int par[40001], rnk[40001];
int find(int x) { return par[x]==x?x:par[x]=find(par[x]); }
bool unite(int a, int b) {
    a=find(a); b=find(b);
    if(a==b) return false;
    if(rnk[a]<rnk[b]) swap(a,b);
    par[b]=a;
    if(rnk[a]==rnk[b]) rnk[a]++;
    return true;
}

int main(){
    int n, m;
    scanf("%d%d",&n,&m);
    vector<int> eu(m+1), ev(m+1), ew(m+1);
    set<pair<int,int>> S;
    for(int i=1;i<=m;i++){
        scanf("%d%d%d",&eu[i],&ev[i],&ew[i]);
        S.insert({ew[i],i});
    }
    int t; scanf("%d",&t);
    while(t--){
        int e, c; scanf("%d%d",&e,&c);
        S.erase({ew[e],e});
        ew[e]=c;
        S.insert({ew[e],e});
        for(int i=1;i<=n;i++) par[i]=i, rnk[i]=0;
        long long ans=0; int cnt=0;
        for(auto&[w,id]:S){
            if(unite(eu[id],ev[id])){ ans+=w; if(++cnt==n-1) break; }
        }
        printf("%lld\n",ans);
    }
}
