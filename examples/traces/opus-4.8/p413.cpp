#include <bits/stdc++.h>
using namespace std;
int n,m;
vector<int> adj[105];
int comp[105];
bool vis[105];
int curLabel;

vector<int> dfs(int u,int p){
    vis[u]=true;
    vector<int> cur; cur.push_back(u);
    for(int w:adj[u]){
        if(w==p||vis[w])continue;
        vector<int> r=dfs(w,u);
        for(int x:r)cur.push_back(x);
    }
    if((int)cur.size()>=2){
        curLabel++;
        for(int x:cur)comp[x]=curLabel;
        return vector<int>();
    }
    return cur;
}

int main(){
    int t; scanf("%d",&t);
    while(t--){
        scanf("%d %d",&n,&m);
        for(int i=1;i<=n;i++){adj[i].clear();vis[i]=false;comp[i]=0;}
        for(int i=0;i<m;i++){int a,b;scanf("%d %d",&a,&b);adj[a].push_back(b);adj[b].push_back(a);}
        curLabel=0;
        vector<int> leftover=dfs(1,0);
        if(!leftover.empty()){
            int u=leftover[0];
            comp[u]=comp[adj[u][0]];
        }
        for(int i=1;i<=n;i++)printf("%d%c",comp[i],i==n?'\n':' ');
    }
    return 0;
}
