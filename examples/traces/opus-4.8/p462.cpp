#include <bits/stdc++.h>
using namespace std;

int n;
struct Edge { int a,b,r,p,id; };
vector<int> par, rnk;
int find(int x){ while(par[x]!=x){ par[x]=par[par[x]]; x=par[x]; } return x; }
bool unite(int x,int y){
    x=find(x); y=find(y);
    if(x==y) return false;
    if(rnk[x]<rnk[y]) swap(x,y);
    par[y]=x;
    if(rnk[x]==rnk[y]) rnk[x]++;
    return true;
}

int main(){
    scanf("%d",&n);
    vector<Edge> e(n);
    // coordinate compress nodes
    vector<int> coords;
    for(int i=0;i<n;i++){
        scanf("%d %d %d %d",&e[i].a,&e[i].b,&e[i].r,&e[i].p);
        e[i].id=i;
        coords.push_back(e[i].a);
        coords.push_back(e[i].b);
    }
    sort(coords.begin(),coords.end());
    coords.erase(unique(coords.begin(),coords.end()),coords.end());
    auto idx=[&](int v){ return (int)(lower_bound(coords.begin(),coords.end(),v)-coords.begin()); };
    for(int i=0;i<n;i++){ e[i].a=idx(e[i].a); e[i].b=idx(e[i].b); }
    
    int m=coords.size();
    par.resize(m); rnk.assign(m,0);
    for(int i=0;i<m;i++) par[i]=i;
    
    // Kruskal order: r desc, p desc
    vector<int> ord(n);
    for(int i=0;i<n;i++) ord[i]=i;
    sort(ord.begin(),ord.end(),[&](int x,int y){
        if(e[x].r!=e[y].r) return e[x].r>e[y].r;
        return e[x].p>e[y].p;
    });
    
    vector<char> kept(n,0);
    long long total=0;
    for(int i=0;i<n;i++){
        int j=ord[i];
        if(unite(e[j].a,e[j].b)){
            kept[j]=1;
            total += e[j].p;
        }
    }
    
    // output order: r descending; within same r, dropped (kept=false) first
    vector<int> out(n);
    for(int i=0;i<n;i++) out[i]=i;
    sort(out.begin(),out.end(),[&](int x,int y){
        if(e[x].r!=e[y].r) return e[x].r>e[y].r;
        if(kept[x]!=kept[y]) return kept[x]<kept[y]; // false(0) first
        return false;
    });
    
    printf("%lld\n",total);
    for(int i=0;i<n;i++){
        printf("%d",e[out[i]].id+1);
        if(i+1<n) printf(" ");
    }
    printf("\n");
    return 0;
}
