#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    struct E{int to,cap,rev;};
    vector<vector<E>> g;
    vector<int> level,it;
    int n;
    Dinic(int n):g(n),level(n),it(n),n(n){}
    void add(int a,int b,int c){
        g[a].push_back({b,c,(int)g[b].size()});
        g[b].push_back({a,0,(int)g[a].size()-1});
    }
    bool bfs(int s,int t){
        fill(level.begin(),level.end(),-1);
        queue<int>q;level[s]=0;q.push(s);
        while(!q.empty()){int v=q.front();q.pop();
            for(auto&e:g[v])if(e.cap>0&&level[e.to]<0){level[e.to]=level[v]+1;q.push(e.to);}
        }
        return level[t]>=0;
    }
    int dfs(int v,int t,int f){
        if(v==t)return f;
        for(int&i=it[v];i<(int)g[v].size();i++){
            E&e=g[v][i];
            if(e.cap>0&&level[v]<level[e.to]){
                int d=dfs(e.to,t,min(f,e.cap));
                if(d>0){e.cap-=d;g[e.to][e.rev].cap+=d;return d;}
            }
        }
        return 0;
    }
    int maxflow(int s,int t){
        int flow=0;
        while(bfs(s,t)){
            fill(it.begin(),it.end(),0);
            int f;
            while((f=dfs(s,t,INT_MAX))>0)flow+=f;
        }
        return flow;
    }
};

int main(){
    int n;
    scanf("%d",&n);
    vector<int> w(n),r(n);
    for(int i=0;i<n;i++)scanf("%d",&w[i]);
    for(int i=0;i<n;i++)scanf("%d",&r[i]);
    vector<vector<int>> a(n,vector<int>(n));
    for(int i=0;i<n;i++)for(int j=0;j<n;j++)scanf("%d",&a[i][j]);

    int maxWins = w[0]+r[0];

    // check feasibility of caps
    for(int i=1;i<n;i++){
        if(maxWins - w[i] < 0){ printf("NO\n"); return 0; }
    }

    // collect intra games among teams 2..n (index 1..n-1)
    // nodes: source=0, team nodes 1..n-1 mapped to 1..(n-1), game nodes after, sink last
    vector<pair<int,int>> games;
    int total=0;
    for(int i=1;i<n;i++)for(int j=i+1;j<n;j++){
        if(a[i][j]>0){ games.push_back({i,j}); total+=a[i][j]; }
    }

    int numTeams = n-1; // teams index 1..n-1
    int G = games.size();
    // node ids: source=0, team k (k=1..n-1) -> k, game g -> numTeams+1+g, sink = numTeams+1+G
    int sink = numTeams+1+G;
    Dinic d(sink+1);
    for(size_t gi=0; gi<games.size(); gi++){
        int i=games[gi].first, j=games[gi].second;
        int gnode = numTeams+1+gi;
        d.add(0, gnode, a[i][j]);
        d.add(gnode, i, 1000000);
        d.add(gnode, j, 1000000);
    }
    for(int i=1;i<n;i++){
        d.add(i, sink, maxWins - w[i]);
    }
    int f = d.maxflow(0,sink);
    if(f==total) printf("YES\n"); else printf("NO\n");
    return 0;
}
