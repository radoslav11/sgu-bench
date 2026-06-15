#include <bits/stdc++.h>
using namespace std;

int main(){
    int N; long long T;
    scanf("%d %lld",&N,&T);
    vector<long long> t(N+1);
    for(int i=1;i<=N;i++) scanf("%lld",&t[i]);
    int M; scanf("%d",&M);
    vector<vector<int>> adj(N+1), pred(N+1);
    for(int i=0;i<M;i++){
        int a,b; scanf("%d %d",&a,&b);
        adj[a].push_back(b);
        pred[b].push_back(a);
    }
    // Tarjan SCC
    vector<int> num(N+1,0), low(N+1,0), comp(N+1,-1);
    vector<bool> onstk(N+1,false);
    stack<int> stk;
    int cnt=0, sccCount=0;
    // iterative tarjan to be safe
    {
        vector<int> callStack, iterIdx;
        for(int s=1;s<=N;s++){
            if(num[s]) continue;
            callStack.push_back(s);
            iterIdx.push_back(0);
            num[s]=low[s]=++cnt; stk.push(s); onstk[s]=true;
            while(!callStack.empty()){
                int u=callStack.back();
                int &ii=iterIdx.back();
                if(ii < (int)adj[u].size()){
                    int v=adj[u][ii++];
                    if(!num[v]){
                        num[v]=low[v]=++cnt; stk.push(v); onstk[v]=true;
                        callStack.push_back(v); iterIdx.push_back(0);
                    } else if(onstk[v]){
                        low[u]=min(low[u], num[v]);
                    }
                } else {
                    if(low[u]==num[u]){
                        int c=sccCount++;
                        while(true){
                            int w=stk.top(); stk.pop(); onstk[w]=false; comp[w]=c;
                            if(w==u) break;
                        }
                    }
                    callStack.pop_back(); iterIdx.pop_back();
                    if(!callStack.empty()){
                        int parent=callStack.back();
                        low[parent]=min(low[parent], low[u]);
                    }
                }
            }
        }
    }
    // size of each scc
    vector<int> sccSize(sccCount,0);
    for(int i=1;i<=N;i++) sccSize[comp[i]]++;
    vector<bool> bad(N+1,false);
    // initial bad: nodes in scc of size>1
    queue<int> bfs;
    for(int i=1;i<=N;i++){
        if(sccSize[comp[i]]>1){
            bad[i]=true;
            bfs.push(i);
        }
    }
    // propagate forward along adj (descendants become bad)
    while(!bfs.empty()){
        int u=bfs.front(); bfs.pop();
        for(int v: adj[u]){
            if(!bad[v]){ bad[v]=true; bfs.push(v); }
        }
    }
    // solvable nodes form a DAG; compute topo order via Kahn
    vector<int> indeg(N+1,0);
    for(int v=1;v<=N;v++){
        if(bad[v]) continue;
        for(int a: pred[v]){
            if(!bad[a]) indeg[v]++;
        }
    }
    vector<int> pos(N+1, 0);
    {
        queue<int> q;
        for(int v=1;v<=N;v++) if(!bad[v] && indeg[v]==0) q.push(v);
        int order=0;
        while(!q.empty()){
            int u=q.front(); q.pop();
            pos[u]=order++;
            for(int w: adj[u]){
                if(bad[w]) continue;
                if(--indeg[w]==0) q.push(w);
            }
        }
    }
    // collect solvable nodes, sort by (t, pos)
    vector<int> nodes;
    for(int v=1;v<=N;v++) if(!bad[v]) nodes.push_back(v);
    sort(nodes.begin(), nodes.end(), [&](int x,int y){
        if(t[x]!=t[y]) return t[x]<t[y];
        return pos[x]<pos[y];
    });
    // greedy selection
    vector<bool> selected(N+1,false);
    long long sum=0;
    vector<int> result;
    for(int v: nodes){
        // check all preds selected
        bool ok=true;
        for(int a: pred[v]){
            if(bad[a]) { ok=false; break; }
            if(!selected[a]){ ok=false; break; }
        }
        if(!ok) continue;
        if(sum + t[v] <= T){
            selected[v]=true;
            sum += t[v];
            result.push_back(v);
        }
    }
    // compute penalty
    long long penalty=0, cum=0;
    for(int v: result){
        cum += t[v];
        penalty += cum;
    }
    printf("%d %lld\n", (int)result.size(), penalty);
    for(size_t i=0;i<result.size();i++){
        if(i) printf(" ");
        printf("%d", result[i]);
    }
    printf("\n");
    return 0;
}
