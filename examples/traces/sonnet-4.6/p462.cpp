#include <bits/stdc++.h>
using namespace std;

struct DSU {
    map<int,int> parent, rank_;
    int find(int x) { if(!parent.count(x)) { parent[x]=x; rank_[x]=0; } return parent[x]==x?x:parent[x]=find(parent[x]); }
    bool unite(int a, int b) { a=find(a); b=find(b); if(a==b) return false; if(rank_[a]<rank_[b]) swap(a,b); parent[b]=a; if(rank_[a]==rank_[b]) rank_[a]++; return true; }
};

int main(){
    int n; scanf("%d",&n);
    vector<int> a(n),b(n),r(n),p(n);
    for(int i=0;i<n;i++) scanf("%d%d%d%d",&a[i],&b[i],&r[i],&p[i]);
    vector<int> idx(n); iota(idx.begin(),idx.end(),0);
    sort(idx.begin(),idx.end(),[&](int x,int y){ return p[x]>p[y]; });
    DSU dsu; vector<bool> inF(n,false);
    for(int i:idx) if(dsu.unite(a[i],b[i])) inF[i]=true;
    long long tot=0; vector<int> ord,ord2;
    for(int i=0;i<n;i++) if(!inF[i]) ord.push_back(i+1); else { tot+=p[i]; ord2.push_back(i+1); }
    for(int x:ord2) ord.push_back(x);
    printf("%lld\n",tot);
    for(int i=0;i<(int)ord.size();i++) printf("%d%c",ord[i]," \n"[i+1==(int)ord.size()]);
}
