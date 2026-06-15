#include <bits/stdc++.h>
using namespace std;

int N, M;
vector<int> adj_head, adj_next, adj_to;
int edgeCnt=0;
void addEdge(int u,int v){
    adj_to[edgeCnt]=v; adj_next[edgeCnt]=adj_head[u]; adj_head[u]=edgeCnt++;
}

vector<int> distA, distB;
vector<char> used, dead;
vector<int> nextN;
int K;
const int INF = 1e9;

bool dfs(int u){
    if(distB[u]==0) return true; // reached a B vertex (layer K)
    for(int e=adj_head[u]; e!=-1; e=adj_next[e]){
        int w=adj_to[e];
        if(used[w]||dead[w]) continue;
        // valid check
        if(distA[w]>=INF||distB[w]>=INF) continue;
        if(distA[w]+distB[w]!=K) continue;
        if(distA[w]!=distA[u]+1) continue;
        if(dfs(w)){
            used[w]=1;
            nextN[u]=w;
            return true;
        }
    }
    dead[u]=1;
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    if(!(cin>>N>>M)) return 0;
    adj_head.assign(N+1,-1);
    adj_next.assign(2*M,0);
    adj_to.assign(2*M,0);
    for(int i=0;i<M;i++){
        int u,v; cin>>u>>v;
        addEdge(u,v); addEdge(v,u);
    }
    int n1; cin>>n1;
    vector<int> A(n1);
    for(int i=0;i<n1;i++) cin>>A[i];
    int n2; cin>>n2;
    vector<int> B(n2);
    for(int i=0;i<n2;i++) cin>>B[i];
    
    distA.assign(N+1, INF);
    distB.assign(N+1, INF);
    
    // BFS from A
    {
        queue<int> q;
        for(int a:A){ if(distA[a]>=INF){distA[a]=0; q.push(a);} }
        while(!q.empty()){
            int u=q.front(); q.pop();
            for(int e=adj_head[u]; e!=-1; e=adj_next[e]){
                int w=adj_to[e];
                if(distA[w]>=INF){ distA[w]=distA[u]+1; q.push(w);}
            }
        }
    }
    // BFS from B
    {
        queue<int> q;
        for(int b:B){ if(distB[b]>=INF){distB[b]=0; q.push(b);} }
        while(!q.empty()){
            int u=q.front(); q.pop();
            for(int e=adj_head[u]; e!=-1; e=adj_next[e]){
                int w=adj_to[e];
                if(distB[w]>=INF){ distB[w]=distB[u]+1; q.push(w);}
            }
        }
    }
    
    // K = min over a in A of distB[a]
    K = INF;
    for(int a:A){ if(distB[a]<K) K=distB[a]; }
    // K should be finite per problem
    
    used.assign(N+1,0);
    dead.assign(N+1,0);
    nextN.assign(N+1,-1);
    
    vector<vector<int>> paths;
    
    // start vertices: a in A with distB[a]==K (so distA=0, layer 0)
    for(int a:A){
        if(distB[a]!=K) continue;
        if(used[a]||dead[a]) continue;
        if(distA[a]!=0) continue; // should be 0 for A vertices
        if(distA[a]+distB[a]!=K) continue;
        if(dfs(a)){
            used[a]=1;
            vector<int> path;
            int cur=a;
            path.push_back(cur);
            while(distB[cur]!=0){
                cur=nextN[cur];
                path.push_back(cur);
            }
            paths.push_back(path);
        }
    }
    
    // output
    cout<<paths.size()<<" "<<K<<"\n";
    for(auto &p:paths){
        for(size_t i=0;i<p.size();i++){
            if(i) cout<<" ";
            cout<<p[i];
        }
        cout<<"\n";
    }
    return 0;
}
