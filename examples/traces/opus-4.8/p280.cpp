#include <bits/stdc++.h>
using namespace std;
const int NEG=-1000000, POS=1000000;
int main(){
    int n,k; scanf("%d %d",&n,&k);
    vector<vector<int>> adj(n+1);
    for(int i=0;i<n-1;i++){int a,b;scanf("%d %d",&a,&b);adj[a].push_back(b);adj[b].push_back(a);}
    vector<int> par(n+1,0), order; order.reserve(n);
    vector<int> need(n+1,0), cover(n+1,POS);
    vector<bool> vis(n+1,false);
    queue<int>q; q.push(1); vis[1]=true;
    while(!q.empty()){int u=q.front();q.pop();order.push_back(u);for(int w:adj[u])if(!vis[w]){vis[w]=true;par[w]=u;q.push(w);}}
    vector<int> centers;
    for(int i=(int)order.size()-1;i>=0;i--){
        int v=order[i];
        if(cover[v]+need[v]<=k) need[v]=NEG;
        if(need[v]==k){centers.push_back(v);cover[v]=0;need[v]=NEG;}
        int p=par[v];
        if(p){
            need[p]=max(need[p], need[v]+1);
            cover[p]=min(cover[p], cover[v]+1);
        } else {
            if(need[v]>=0){centers.push_back(v);}
        }
    }
    printf("%d\n",(int)centers.size());
    for(int c:centers)printf("%d\n",c);
}
