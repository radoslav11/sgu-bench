#include <bits/stdc++.h>
using namespace std;
int v,e,n;
bool adj[105][105];
int grp[105];
bool used[105];
bool checkGroups(vector<array<int,3>>& groups);
bool tryPartition(int gi, vector<array<int,3>>& groups, int start);
bool solve(){
    if(v%3!=0) return false;
    n=v/3;
    vector<array<int,3>> groups(n);
    return tryPartition(0,groups,1);
}
bool tryPartition(int gi, vector<array<int,3>>& groups, int start){
    if(gi==n) return checkGroups(groups);
    int a=-1;
    for(int i=start;i<=v;i++) if(!used[i]){a=i;break;}
    if(a==-1) return false;
    used[a]=true;
    for(int b=a+1;b<=v;b++){if(!used[b]&&!adj[a][b]){
        used[b]=true;
        for(int c=b+1;c<=v;c++){if(!used[c]&&!adj[a][c]&&!adj[b][c]){
            used[c]=true;
            groups[gi]={a,b,c};
            if(tryPartition(gi+1,groups,a+1)) return true;
            used[c]=false;
        }}
        used[b]=false;
    }}
    used[a]=false;
    return false;
}
bool checkGroups(vector<array<int,3>>& groups){
    // non-edges between different groups must form union of complete bipartite graphs
    // use union-find or coloring: each vertex gets a "literal id"
    // two vertices in different groups with no edge must be "contradictory"
    // contradictory means same variable, opposite sign
    // so: define graph where u-v if no edge and different group
    // this graph must be a union of complete bipartite graphs (bicliques)
    // and no vertex is in two different bicliques
    int lit[105]; memset(lit,-1,sizeof(lit));
    // For each connected component of the "contradiction graph", check it's bipartite
    // and assign literals
    int litcnt=0;
    int side[105]; memset(side,-1,sizeof(side));
    for(int i=1;i<=v;i++){
        if(side[i]!=-1) continue;
        // BFS
        queue<int> q; q.push(i); side[i]=0; lit[i]=litcnt;
        while(!q.empty()){
            int u=q.front();q.pop();
            for(int w=1;w<=v;w++){
                if(w==u) continue;
                // check different group
                bool diff=true;
                for(auto& g:groups) if(g[0]==u||g[1]==u||g[2]==u){if(g[0]==w||g[1]==w||g[2]==w) diff=false;}
                if(!diff) continue;
                if(!adj[u][w]){ // contradiction
                    if(side[w]==-1){side[w]=1-side[u];lit[w]=litcnt;q.push(w);}
                    else if(side[w]==side[u]) return false;
                }
            }
        }
        litcnt++;
    }
    // Now verify: if two vertices have same literal and same side, they must be connected (if in different groups)
    for(int u=1;u<=v;u++) for(int w=u+1;w<=v;w++){
        bool diff=true;
        for(auto& g:groups) if(g[0]==u||g[1]==u||g[2]==u){if(g[0]==w||g[1]==w||g[2]==w) diff=false;}
        if(!diff) continue;
        if(lit[u]==lit[w]&&side[u]==side[w]&&!adj[u][w]) return false;
        if(lit[u]==lit[w]&&side[u]!=side[w]&&adj[u][w]) return false;
        if(lit[u]!=lit[w]&&!adj[u][w]) return false;
    }
    return true;
}
int main(){
    cin>>v>>e;
    for(int i=0;i<e;i++){int a,b;cin>>a>>b;adj[a][b]=adj[b][a]=true;}
    cout<<(solve()?"YES":"NO");
}
