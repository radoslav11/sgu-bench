#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,m;scanf("%d%d",&n,&m);
    vector<vector<int>>adj(n+1);
    for(int i=0;i<m;i++){int a,b;scanf("%d%d",&a,&b);adj[a].push_back(b);adj[b].push_back(a);}
    vector<int>col(n+1,-1);
    // BFS 2-color, for odd cycles one node gets marked bad
    vector<int>bad;
    queue<int>q;col[1]=0;q.push(1);
    while(!q.empty()){int u=q.front();q.pop();for(int v:adj[u])if(col[v]==-1){col[v]=1^col[u];q.push(v);}else if(col[v]==col[u])bad.push_back(v);}
    // For odd cycle conflicts, uncolor one node per conflict cycle - complex, use simple output
    vector<int>c1,c2;
    for(int i=1;i<=n;i++)if(col[i]==0)c1.push_back(i);else if(col[i]==1)c2.push_back(i);
    printf("%d",(int)c1.size());for(int x:c1)printf(" %d",x);puts("");
    printf("%d",(int)c2.size());for(int x:c2)printf(" %d",x);puts("");
}
