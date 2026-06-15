#include<bits/stdc++.h>
using namespace std;
int N,M;
char g[20][20];
int comp[20][20];
int dx[]={0,0,1,-1};
int dy[]={1,-1,0,0};
int main(){
    cin>>N>>M;
    for(int i=0;i<N;i++) cin>>g[i];
    memset(comp,-1,sizeof(comp));
    int nc=0;
    for(int i=0;i<N;i++)for(int j=0;j<M;j++)if(g[i][j]=='X'&&comp[i][j]==-1){
        queue<pair<int,int>>q;q.push({i,j});comp[i][j]=nc;
        while(!q.empty()){auto[x,y]=q.front();q.pop();for(int d=0;d<4;d++){int nx=x+dx[d],ny=y+dy[d];if(nx>=0&&nx<N&&ny>=0&&ny<M&&g[nx][ny]=='X'&&comp[nx][ny]==-1){comp[nx][ny]=nc;q.push({nx,ny});}}}
        nc++;
    }
    // Steiner tree: terminals = ocean(virtual, index nc) + crater components
    // State: dist[cell][mask] = min cost
    // For each pair of crater components (ci,cj), solve Steiner tree with terminals {ocean, ci, cj}
    // Ocean = virtual node; ocean-adjacent = border cells and cells adjacent to outside
    // A '.' cell is adjacent to ocean if it's on border or has neighbor outside grid
    // Actually ocean adjacency: cell (i,j) is adjacent to ocean if i==0||i==N-1||j==0||j==M-1
    // (since problem says no X touches ocean, and ocean is outside)
    
    // For Steiner tree with 3 terminals T={0=ocean,1=ci,2=cj}:
    // dp[cell][mask] = min '.' squares used in a Steiner subtree rooted at cell covering terminals in mask
    // But this needs a tree structure; use the standard Steiner tree DP on graphs
    
    // Standard Steiner tree on graph:
    // dp[S][v] = min cost tree spanning terminal set S, containing vertex v
    // Transitions:
    // 1. dp[S][v] = min over S1,S2 partition of S: dp[S1][v]+dp[S2][v]
    // 2. Relax by shortest path (Dijkstra)
    
    // Vertices: all '.' cells + virtual ocean node (index N*M)
    // Terminals: ocean node, and one '.' cell adjacent to crater ci, one adjacent to crater cj
    // Wait, craters are 'X' cells; the pipe connects to crater via edge, so a '.' cell adjacent to 'X' cell of component ci is "connected" to ci
    
    // Let me redefine: 
    // Node = '.' cell or virtual ocean node
    // Edge between two '.' cells if adjacent, cost 1 (we count destination)
    // Edge from ocean node to border '.' cells, cost 0
    // A node v is "touching crater ci" if v is a '.' cell adjacent to some 'X' cell of component ci
    // Terminal 0 = ocean node
    // Terminal 1 = "any node touching ci" -> we can handle by adding virtual terminal nodes
    
    // Easier: for each crater component c, add a virtual node vc connected to all '.' cells adjacent to c's X cells, cost 0
    // Similarly ocean virtual node connected to border '.' cells cost 0
    // Then Steiner tree on {ocean_virtual, vc1, vc2}
    
    // With nc up to ~56 components, nc*(nc-1)/2 pairs, each Steiner tree O(3^k * V + 2^k * V * log V) with k=3 terminals
    // k=3: 3^3=27, 2^3=8, V=225+nc+1~280, very fast
    
    // Let me implement this
    
    // Graph nodes: 0..N*M-1 = grid cells, N*M = ocean virtual, N*M+1..N*M+nc = crater virtuals
    int V = N*M + 1 + nc;
    int ocean = N*M;
    // crater virtual node for component c = N*M+1+c
    
    // Build adjacency: list of (neighbor, cost)
    // cost of entering a '.' cell = 1, entering virtual nodes = 0
    vector<vector<pair<int,int>>> adj(V);
    auto id=[&](int r,int c){return r*M+c;};
    for(int i=0;i<N;i++)for(int j=0;j<M;j++){
        if(g[i][j]=='.'){
            // connect to adjacent '.' cells
            for(int d=0;d<4;d++){
                int ni=i+dx[d],nj=j+dy[d];
                if(ni<0||ni>=N||nj<0||nj>=M){
                    // adjacent to ocean
                    adj[ocean].push_back({id(i,j),1}); // entering this cell costs 1
                    adj[id(i,j)].push_back({ocean,0}); // going to ocean costs 0
                } else if(g[ni][nj]=='.'){
                    adj[id(i,j)].push_back({id(ni,nj),1});
                } else {
                    // adjacent to X cell of component comp[ni][nj]
                    int c=comp[ni][nj];
                    int cv=N*M+1+c;
                    adj[cv].push_back({id(i,j),1}); // entering . cell costs 1
                    adj[id(i,j)].push_back({cv,0}); // going to crater virtual costs 0
                }
            }
        }
    }
    // Also: if a '.' cell is adjacent to ocean (border), connect bidirectionally already done above
    // But multiple edges to ocean from same cell? That's fine, just duplicates
    
    // Now for each pair (ci,cj) with ci<cj, solve Steiner tree with terminals {ocean, cv_ci, cv_cj}
    // terminals indexed 0,1,2
    // dp[mask][v] = min cost
    
    const int INF=1e9;
    int bestCost=INF;
    vector<vector<int>> bestDP;
    int bestCI=-1,bestCJ=-1;
    
    // Also consider single path: one crater ci with min cost (just one pipe touching one crater - NO, need at least 2)
    // Actually "two independent pipes" means two separate pipes each connecting one crater to ocean
    // So total cost = cost(ci) + cost(cj)
    // "one pipe connecting two craters to ocean" = Steiner tree with 3 terminals
    // We want minimum of both options
    
    // For option A (two independent pipes):
    // min cost for each component alone = Steiner tree with 2 terminals {ocean, cv_ci}
    // For each ci, compute minCost[ci]
    
    vector<int> minCostAlone(nc,INF);
    vector<vector<int>> dpAlone(nc, vector<int>(V,INF));
    
    for(int ci=0;ci<nc;ci++){
        // 2 terminals: ocean(0), cv_ci(1)
        // dp[mask][v]
        // mask in {1,2,3}, terminals[0]=ocean, terminals[1]=N*M+1+ci
        int t0=ocean, t1=N*M+1+ci;
        vector<vector<int>> dp(4, vector<int>(V,INF));
        dp[1][t0]=0;
        dp[2][t1]=0;
        // For each node adjacent to t0: already handled via adj[t0]
        // Dijkstra for mask=1,2
        for(int mask=1;mask<=2;mask++){
            priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>> pq;
            for(int v=0;v<V;v++) if(dp[mask][v]<INF) pq.push({dp[mask][v],v});
            while(!pq.empty()){
                auto[cost,v]=pq.top();pq.pop();
                if(cost>dp[mask][v]) continue;
                for(auto[u,w]:adj[v]){
                    if(dp[mask][v]+w<dp[mask][u]){
                        dp[mask][u]=dp[mask][v]+w;
                        pq.push({dp[mask][u],u});
                    }
                }
            }
        }
        // Combine for mask=3
        for(int v=0;v<V;v++){
            dp[3][v]=min(dp[3][v],dp[1][v]+dp[2][v]);
        }
        // Dijkstra for mask=3
        {
            priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>> pq;
            for(int v=0;v<V;v++) if(dp[3][v]<INF) pq.push({dp[3][v],v});
            while(!pq.empty()){
                auto[cost,v]=pq.top();pq.pop();
                if(cost>dp[3][v]) continue;
                for(auto[u,w]:adj[v]){
                    if(dp[3][v]+w<dp[3][u]){
                        dp[3][u]=dp[3][v]+w;
                        pq.push({dp[3][u],u});
                    }
                }
            }
        }
        // min cost for ci = min over v of dp[3][v]
        for(int v=0;v<V;v++) minCostAlone[ci]=min(minCostAlone[ci],dp[3][v]);
        dpAlone[ci]=dp[3];
    }
    
    // Option A: pick two best ci,cj with min minCostAlone[ci]+minCostAlone[cj]
    // Find two smallest minCostAlone values
    vector<int> sortedIdx(nc);iota(sortedIdx.begin(),sortedIdx.end(),0);
    sort(sortedIdx.begin(),sortedIdx.end(),[&](int a,int b){return minCostAlone[a]<minCostAlone[b];});
    
    int optA_cost = INF;
    int optA_ci=-1,optA_cj=-1;
    if(nc>=2 && minCostAlone[sortedIdx[0]]<INF && minCostAlone[sortedIdx[1]]<INF){
        optA_cost = minCostAlone[sortedIdx[0]]+minCostAlone[sortedIdx[1]];
        optA_ci=sortedIdx[0]; optA_cj=sortedIdx[1];
    }
    
    // Option B: Steiner tree with 3 terminals for each pair
    // We'll compute for all pairs and find best
    // But nc can be up to ~56, pairs up to ~1540, each needs O(8*V*logV) Dijkstra -> feasible
    
    int optB_cost=INF;
    int optB_ci=-1,optB_cj=-1;
    vector<vector<int>> optB_dp;
    
    for(int ci=0;ci<nc;ci++) for(int cj=ci+1;cj<nc;cj++){
        int t0=ocean, t1=N*M+1+ci, t2=N*M+1+cj;
        int terms[3]={t0,t1,t2};
        vector<vector<int>> dp(8, vector<int>(V,INF));
        dp[1][t0]=0;dp[2][t1]=0;dp[4][t2]=0;
        for(int mask=1;mask<8;mask++){
            if(__builtin_popcount(mask)==1){
                // already initialized, run Dijkstra
                priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>> pq;
                for(int v=0;v<V;v++) if(dp[mask][v]<INF) pq.push({dp[mask][v],v});
                while(!pq.empty()){
                    auto[cost,v]=pq.top();pq.pop();
                    if(cost>dp[mask][v]) continue;
                    for(auto[u,w]:adj[v]){
                        if(dp[mask][v]+w<dp[mask][u]){
                            dp[mask][u]=dp[mask][v]+w;
                            pq.push({dp[mask][u],u});
                        }
                    }
                }
            } else {
                // combine subsets
                for(int sub=1;sub<mask;sub++){
                    if((sub&mask)==sub){
                        int comp2=mask^sub;
                        if(sub<comp2) continue; // avoid double counting... actually do all
                        for(int v=0;v<V;v++){
                            if(dp[sub][v]<INF&&dp[comp2][v]<INF)
                                dp[mask][v]=min(dp[mask][v],dp[sub][v]+dp[comp2][v]);
                        }
                    }
                }
                // Dijkstra
                priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>> pq;
                for(int v=0;v<V;v++) if(dp[mask][v]<INF) pq.push({dp[mask][v],v});
                while(!pq.empty()){
                    auto[cost,v]=pq.top();pq.pop();
                    if(cost>dp[mask][v]) continue;
                    for(auto[u,w]:adj[v]){
                        if(dp[mask][v]+w<dp[mask][u]){
                            dp[mask][u]=dp[mask][v]+w;
                            pq.push({dp[mask][u],u});
                        }
                    }
                }
            }
        }
        int cost=*min_element(dp[7].begin(),dp[7].end());
        if(cost<optB_cost){optB_cost=cost;optB_ci=ci;optB_cj=cj;optB_dp=dp;}
    }
    
    // Now reconstruct
    // Choose best option
    // For reconstruction, I need to trace back which cells are used
    // This is the hard part...
    
    // Let me do reconstruction by re-running BFS/Dijkstra and tracking predecessors
    // Or: mark cells used by checking if dp[mask][v] + dp[fullmask^mask][v] == dp[fullmask][best_v] for some v
    
    // Actually for reconstruction of Steiner tree:
    // Standard approach: for each mask, find the optimal v, then recurse
    
    // Let me implement reconstruction
    // Given dp table and the best root vertex for full mask, trace back
    
    // For the Steiner tree reconstruction:
    // We have dp[mask][v] for all mask, v
    // For mask with one bit: trace shortest path from terminal to v (dijkstra predecessors)
    // For composite mask: find v* = argmin dp[mask][v], find split sub1,sub2 such that dp[sub1][v*]+dp[sub2][v*]=dp[mask][v*], recurse
    // Then combine paths
    
    // This is complex. Let me use a simpler approach:
    // Just mark '*' on cells that are in the optimal Steiner tree
    // Reconstruction by BFS on the DP values
    
    // I'll implement reconstruction using predecessor arrays from Dijkstra
    
    // Given time constraints, let me implement a cleaner solution
    
    // Actually, let me think differently for reconstruction:
    // Once we have dp[mask][v] for all v,mask, we can reconstruct as follows:
    // Start with full_mask, find v with min dp[full_mask][v]
    // Try all splits into sub1 | sub2 = full_mask, sub1 & sub2 = 0
    // If dp[sub1][v] + dp[sub2][v] = dp[full_mask][v], recurse on (sub1,v) and (sub2,v)
    // For singleton masks, trace shortest path via predecessors
    
    // I need predecessor arrays for each Dijkstra run
    // Let me restructure: compute everything with predecessors
    
    // This is getting complex. Let me write a cleaner full solution.
    
    cout << "TODO" << endl;
    return 0;
}
