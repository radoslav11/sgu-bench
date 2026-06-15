#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> adjList;
vector<vector<char>> adj;
vector<char> visited;
vector<int> pathArr;
int startV;

// temp arrays
vector<int> compId;

bool connectedPrune(int cur, int depth){
    // nodes: unvisited + {cur, startV}; check all in one component
    // collect unvisited
    // BFS from cur over unvisited and start
    // mark
    static vector<int> stk;
    static vector<char> seen;
    seen.assign(n,0);
    stk.clear();
    stk.push_back(cur); seen[cur]=1;
    // start should also be reachable; include start as node
    while(!stk.empty()){
        int u=stk.back(); stk.pop_back();
        for(int w: adjList[u]){
            if(seen[w]) continue;
            // w is reachable if unvisited, or w==startV (open end)
            if(!visited[w] || w==startV){
                seen[w]=1;
                // only continue expanding through unvisited nodes (not start, start is dead-end except closing)
                if(!visited[w]) stk.push_back(w);
                else if(w==startV){
                    // start: don't expand further (its other edges fixed)
                }
            }
        }
    }
    // all unvisited must be seen, and start must be seen
    if(!seen[startV]) return false;
    for(int u=0;u<n;u++) if(!visited[u] && !seen[u]) return false;
    return true;
}

bool dfs(int cur, int depth){
    if(depth==n){
        return adj[cur][startV];
    }
    // pruning: each unvisited vertex needs avail>=2
    for(int u=0;u<n;u++){
        if(visited[u]) continue;
        int av=0;
        for(int w: adjList[u]){
            if(!visited[w]) av++;
            else if(w==cur) av++;
            else if(w==startV) av++;
            if(av>=2) break;
        }
        if(av<2) return false;
    }
    // connectivity prune
    if(!connectedPrune(cur,depth)) return false;
    // candidates: unvisited neighbors of cur
    // order by unvisited-degree ascending (Warnsdorff)
    static vector<pair<int,int>> cands;
    cands.clear();
    for(int w: adjList[cur]){
        if(!visited[w]){
            int d=0;
            for(int x: adjList[w]) if(!visited[x]) d++;
            cands.push_back({d,w});
        }
    }
    if(cands.empty()) return false;
    sort(cands.begin(),cands.end());
    for(auto&pr: cands){
        int w=pr.second;
        visited[w]=1;
        pathArr[depth]=w;
        if(dfs(w,depth+1)) return true;
        visited[w]=0;
    }
    return false;
}

int main(){
    int f;
    if(!(cin>>f)) return 0;
    map<string, int> idx;
    vector<array<string,3>> pts;
    vector<pair<int,int>> edges;
    for(int i=0;i<f;i++){
        int k; cin>>k;
        vector<int> facetIdx(k);
        for(int j=0;j<k;j++){
            string a,b,c;
            cin>>a>>b>>c;
            string key=a+" "+b+" "+c;
            auto it=idx.find(key);
            int id;
            if(it==idx.end()){
                id=pts.size();
                idx[key]=id;
                pts.push_back({a,b,c});
            } else id=it->second;
            facetIdx[j]=id;
        }
        for(int j=0;j<k;j++){
            int u=facetIdx[j];
            int v=facetIdx[(j+1)%k];
            if(u!=v) edges.push_back({u,v});
        }
    }
    n=pts.size();
    adj.assign(n, vector<char>(n,0));
    for(auto&e: edges){
        adj[e.first][e.second]=1;
        adj[e.second][e.first]=1;
    }
    adjList.assign(n, {});
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if(adj[i][j]) adjList[i].push_back(j);
    
    // quick checks
    bool ok=true;
    if(n<3) ok=false;
    for(int i=0;i<n && ok;i++) if((int)adjList[i].size()<2) ok=false;
    
    if(ok){
        visited.assign(n,0);
        pathArr.assign(n,0);
        startV=0;
        visited[0]=1;
        pathArr[0]=0;
        ok=dfs(0,1);
    }
    
    if(!ok){
        cout<<"No\n";
    } else {
        cout<<"Yes\n";
        cout<<n<<"\n";
        for(int i=0;i<n;i++){
            int id=pathArr[i];
            cout<<pts[id][0]<<" "<<pts[id][1]<<" "<<pts[id][2]<<"\n";
        }
    }
    return 0;
}
