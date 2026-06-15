#include <bits/stdc++.h>
using namespace std;
// [Full solution with structural analysis]
int main(){
    int N,M; cin>>N>>M;
    vector<set<int>> adj(N+1);
    for(int i=0;i<M;i++){int u,v;cin>>u>>v;adj[u].insert(v);adj[v].insert(u);}
    // Try to find hamiltonian cycle by structure
    // degree-2 vertices are bridges between cliques
    vector<int> deg(N+1);
    for(int i=1;i<=N;i++) deg[i]=adj[i].size();
    // Find cliques: connected components of high-degree vertices
    vector<int> comp(N+1,-1);
    int nc=0;
    for(int i=1;i<=N;i++) if(deg[i]>2&&comp[i]==-1){
        queue<int> q; q.push(i); comp[i]=nc;
        while(!q.empty()){int v=q.front();q.pop();
            for(int w:adj[v]) if(deg[w]>2&&comp[w]==-1){comp[w]=nc;q.push(w);}}
        nc++;
    }
    // each degree-2 vertex connects two cliques (or same?)
    // meta graph
    vector<vector<pair<int,int>>> meta(nc); // meta[c] = {other_clique, deg2_vertex}
    for(int u=1;u<=N;u++) if(deg[u]==2){
        auto it=adj[u].begin(); int a=*it++;int b=*it;
        int ca=comp[a],cb=comp[b];
        if(ca==cb){cout<<-1;return 0;}
        meta[ca].push_back({cb,u});
        meta[cb].push_back({ca,u});
    }
    // Need Hamiltonian cycle in meta graph
    // Each meta node = clique, each meta edge = degree-2 vertex
    // For Hamiltonian cycle in meta: visit each clique once
    // Within each clique, we enter via one boundary vertex and exit via another
    // and traverse all clique vertices as a path
    // Find euler-like cycle? No, Hamiltonian in meta
    // Since cliques can be large, but meta graph might be small... no, nc can be large
    // Let's try: meta graph must have a Hamiltonian cycle
    // For each clique, exactly 2 meta edges used
    // This is itself a Hamiltonian cycle problem on meta graph...
    // But meta graph has special structure too?
    // Let me think differently - just try to construct greedily
    // Actually for the meta graph: each node must have degree >= 2 in meta
    // and we need a Hamiltonian cycle
    // Given the strange graph properties, meta graph might also be "nice"
    
    // Let's find a Hamiltonian cycle in meta graph via DFS with backtracking
    // nc can be up to 10000... 
    // But maybe meta graph is a simple cycle itself?
    
    // Check if meta graph is 2-regular (each clique has exactly 2 degree-2 neighbors)
    // If so, it's a union of cycles, and we need it to be a single cycle
    
    for(int c=0;c<nc;c++){
        if((int)meta[c].size()!=2){cout<<-1;return 0;}
    }
    // meta is 2-regular, find if single cycle
    vector<bool> vis(nc,false);
    vector<int> order; // cliques in order
    vector<int> bridge_used; // degree-2 vertex used to go from order[i] to order[i+1]
    order.push_back(0); vis[0]=true;
    int cur=0, prev_bridge=-1;
    // pick first neighbor
    int next_c=meta[0][0].first, next_b=meta[0][0].second;
    while(true){
        bridge_used.push_back(next_b);
        order.push_back(next_c);
        if(next_c==0) break;
        if(vis[next_c]){cout<<-1;return 0;}
        vis[next_c]=true;
        int nb=next_b;
        // from next_c, go to the other neighbor (not via nb)
        if(meta[next_c][0].second==nb){next_b=meta[next_c][1].second;next_c=meta[next_c][1].first;}
        else{next_b=meta[next_c][0].second;next_c=meta[next_c][0].first;}
    }
    if((int)order.size()-1!=nc){cout<<-1;return 0;}
    // Now construct actual Hamiltonian cycle
    // order[0..nc-1] are cliques, bridge_used[i] connects order[i] to order[i+1]
    // For clique order[i], we enter from bridge_used[i-1] and exit to bridge_used[i]
    // entry vertex in clique: the neighbor of bridge_used[i-1] that's in clique order[i]
    // exit vertex in clique: the neighbor of bridge_used[i] that's in clique order[i]
    // Need Hamiltonian path in clique from entry to exit
    // Clique: all vertices are connected, so any path works
    // Collect clique vertices
    vector<vector<int>> clique_verts(nc);
    for(int i=1;i<=N;i++) if(deg[i]>2) clique_verts[comp[i]].push_back(i);
    
    vector<int> result;
    int n2=nc;
    for(int i=0;i<n2;i++){
        int cl=order[i];
        int b_in=bridge_used[(i-1+n2)%n2]; // bridge coming in
        int b_out=bridge_used[i]; // bridge going out
        // find entry and exit in clique
        int entry=-1,exit_v=-1;
        for(int v:adj[b_in]) if(comp[v]==cl){entry=v;break;}
        for(int v:adj[b_out]) if(comp[v]==cl){exit_v=v;break;}
        // add bridge b_in first, then path through clique from entry to exit
        result.push_back(b_in);
        // path in clique: entry, then all others except exit, then exit
        vector<int>& cv=clique_verts[cl];
        // entry to exit hamiltonian path in clique
        // just: entry, [others in any order], exit
        result.push_back(entry);
        for(int v:cv) if(v!=entry&&v!=exit_v) result.push_back(v);
        result.push_back(exit_v);
    }
    // output
    for(int i=0;i<(int)result.size();i++) cout<<result[i]<<" \n"[i+1==(int)result.size()];
}
