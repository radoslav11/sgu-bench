#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct MCMF {
    struct Edge { int to, cap, cost, flow; };
    vector<Edge> edges;
    vector<vector<int>> g;
    int n;
    MCMF(int n): n(n), g(n) {}
    void addEdge(int u, int v, int cap, int cost) {
        g[u].push_back(edges.size());
        edges.push_back({v, cap, cost, 0});
        g[v].push_back(edges.size());
        edges.push_back({u, 0, -cost, 0});
    }
    int s, t;
    vector<int> dist, inq, prevv;
    bool spfa() {
        dist.assign(n, INF);
        inq.assign(n, 0);
        prevv.assign(n, -1);
        deque<int> q;
        dist[s] = 0; q.push_back(s); inq[s]=1;
        while(!q.empty()){
            int u = q.front(); q.pop_front(); inq[u]=0;
            for(int id : g[u]){
                Edge &e = edges[id];
                if(e.cap - e.flow > 0 && dist[u] + e.cost < dist[e.to]){
                    dist[e.to] = dist[u] + e.cost;
                    prevv[e.to] = id;
                    if(!inq[e.to]){ inq[e.to]=1; q.push_back(e.to);}
                }
            }
        }
        return dist[t] < INF;
    }
    // returns {flow, cost}
    pair<int,int> run(int s_, int t_){
        s=s_; t=t_;
        int flow=0, cost=0;
        while(spfa()){
            // find min cap along path
            int f = INF;
            for(int v=t; v!=s; ){
                int id = prevv[v];
                f = min(f, edges[id].cap - edges[id].flow);
                v = edges[id^1].to;
            }
            for(int v=t; v!=s; ){
                int id = prevv[v];
                edges[id].flow += f;
                edges[id^1].flow -= f;
                v = edges[id^1].to;
            }
            flow += f;
            cost += f * dist[t];
        }
        return {flow, cost};
    }
};

int main(){
    int N, M;
    scanf("%d %d", &N, &M);
    // bipartite: left nodes 1..N, right nodes N+1..2N
    // source 0, sink 2N+1
    int S = 0, T = 2*N+1;
    MCMF mc(2*N+2);
    for(int i=1;i<=N;i++){
        mc.addEdge(S, i, 1, 0);
        mc.addEdge(N+i, T, 1, 0);
    }
    // store edge ids for matching reconstruction
    vector<tuple<int,int,int>> roads;
    for(int i=0;i<M;i++){
        int a,b,c;
        scanf("%d %d %d", &a,&b,&c);
        mc.addEdge(a, N+b, 1, c);
        roads.push_back({a,b,c});
    }
    
    auto res = mc.run(S, T);
    int matched = res.first;
    int cost = res.second;
    int K = N - matched;
    
    // reconstruct matching: for each left node, find which right node it connects via flow
    // next[a] = b
    vector<int> nxt(N+1, 0);
    vector<int> indeg(N+1, 0);
    // examine edges
    for(int id=0; id<(int)mc.edges.size(); id++){
        auto &e = mc.edges[id];
        // forward edges with cap>0 and flow>0 between left and right
        if(e.cap>0 && e.flow>0){
            int u = mc.edges[id^1].to; // from
            int v = e.to;
            if(u>=1 && u<=N && v>=N+1 && v<=2*N){
                int b = v - N;
                nxt[u] = b;
                indeg[b]++;
            }
        }
    }
    
    printf("%d %d\n", K, cost);
    // start nodes: those with indeg 0
    for(int i=1;i<=N;i++){
        if(indeg[i]==0){
            vector<int> path;
            int cur = i;
            while(cur){
                path.push_back(cur);
                cur = nxt[cur];
            }
            printf("%d", (int)path.size());
            for(int x : path) printf(" %d", x);
            printf("\n");
        }
    }
    
    return 0;
}
