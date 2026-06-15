#include <bits/stdc++.h>
using namespace std;

int par[300005];
vector<unordered_set<int>> out_, in_;

int find(int x){
    while(par[x]!=x){ par[x]=par[par[x]]; x=par[x]; }
    return x;
}

int mergeRoots(int ra,int rb){
    if(out_[ra].size()+in_[ra].size() < out_[rb].size()+in_[rb].size()) swap(ra,rb);
    int big=ra, small=rb;
    // outgoing edges of small: small -> x
    for(int x: out_[small]){
        int target = (x==small)? big : x;
        if(x!=small && x!=big) in_[x].erase(small);
        out_[big].insert(target);
        if(target!=big) in_[target].insert(big);
        else { out_[big].insert(big); in_[big].insert(big); }
    }
    // incoming edges of small: x -> small
    for(int x: in_[small]){
        if(x==small){
            out_[big].insert(big); in_[big].insert(big);
            continue;
        }
        if(x==big){
            out_[big].erase(small);
            out_[big].insert(big); in_[big].insert(big);
        } else {
            out_[x].erase(small);
            out_[x].insert(big);
            in_[big].insert(x);
        }
    }
    // remove stale references to small inside big's own sets
    out_[big].erase(small);
    in_[big].erase(small);
    par[small]=big;
    out_[small].clear();
    in_[small].clear();
    return big;
}

int main(){
    int N,M;
    scanf("%d %d",&N,&M);
    int maxNodes = N + 200005 + 5;
    out_.assign(maxNodes, unordered_set<int>());
    in_.assign(maxNodes, unordered_set<int>());
    for(int i=0;i<maxNodes;i++) par[i]=i;
    for(int i=0;i<M;i++){
        int a,b; scanf("%d %d",&a,&b);
        out_[a].insert(b);
        in_[b].insert(a);
    }
    int Q; scanf("%d",&Q);
    int nextId = N+1;
    string output;
    output.reserve(Q*4);
    for(int q=0;q<Q;q++){
        int t,a,b; scanf("%d %d %d",&t,&a,&b);
        if(t==1){
            int ra=find(a), rb=find(b);
            if(out_[ra].count(rb)) output+="YES\n";
            else output+="NO\n";
        } else {
            int ra=find(a), rb=find(b);
            int root;
            if(ra==rb) root=ra;
            else root=mergeRoots(ra,rb);
            int nid=nextId++;
            par[nid]=root;
        }
    }
    printf("%s", output.c_str());
    return 0;
}
