#include <bits/stdc++.h>
using namespace std;

struct Edge { int to, cap, flow; };
vector<Edge> edges;
vector<int> g[205];
int level[205], it[205];
int S, T, N;

void addEdge(int u, int v, int cap) {
    g[u].push_back(edges.size());
    edges.push_back({v, cap, 0});
    g[v].push_back(edges.size());
    edges.push_back({u, 0, 0});
}

bool bfs() {
    fill(level, level+N+2, -1);
    queue<int> q;
    level[S]=0; q.push(S);
    while(!q.empty()){
        int u=q.front(); q.pop();
        for(int id: g[u]){
            if(edges[id].cap-edges[id].flow>0 && level[edges[id].to]<0){
                level[edges[id].to]=level[u]+1;
                q.push(edges[id].to);
            }
        }
    }
    return level[T]>=0;
}

int dfs(int u, int pushed){
    if(u==T||pushed==0) return pushed;
    for(int &i=it[u]; i<(int)g[u].size(); i++){
        int id=g[u][i];
        int v=edges[id].to;
        if(level[v]==level[u]+1 && edges[id].cap-edges[id].flow>0){
            int d=dfs(v, min(pushed, edges[id].cap-edges[id].flow));
            if(d>0){
                edges[id].flow+=d;
                edges[id^1].flow-=d;
                return d;
            }
        }
    }
    return 0;
}

int maxflow(){
    int flow=0;
    while(bfs()){
        fill(it, it+N+2, 0);
        while(int f=dfs(S, INT_MAX)) flow+=f;
    }
    return flow;
}

int main(){
    int n, m;
    scanf("%d %d", &n, &m);
    N = n; // nodes 1..n, S=0, T=n+1
    S = 0; T = n+1;
    vector<int> lo(m), edgeId(m);
    vector<int> excess(n+1, 0);
    for(int k=0;k<m;k++){
        int i,j,l,c;
        scanf("%d %d %d %d",&i,&j,&l,&c);
        lo[k]=l;
        edgeId[k]=edges.size();
        addEdge(i, j, c-l);
        excess[i]-=l;
        excess[j]+=l;
    }
    int totalNeed=0;
    for(int v=1;v<=n;v++){
        if(excess[v]>0){ addEdge(S, v, excess[v]); totalNeed+=excess[v]; }
        else if(excess[v]<0){ addEdge(v, T, -excess[v]); }
    }
    int f=maxflow();
    if(f!=totalNeed){
        printf("NO\n");
        return 0;
    }
    printf("YES\n");
    for(int k=0;k<m;k++){
        int id=edgeId[k];
        printf("%d\n", edges[id].flow + lo[k]);
    }
    return 0;
}
