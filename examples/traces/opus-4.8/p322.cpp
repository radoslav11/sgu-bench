#include <bits/stdc++.h>
using namespace std;

int N;
long long keyOf(int a,int b){ if(a>b)swap(a,b); return (long long)a*100000+b; }

int main(){
    scanf("%d",&N);
    set<long long> target; // tree1 edges
    for(int i=0;i<N-1;i++){int a,b;scanf("%d %d",&a,&b);target.insert(keyOf(a,b));}
    vector<set<int>> adj(N+1);
    set<long long> cur; // tree2 edges
    for(int i=0;i<N-1;i++){int a,b;scanf("%d %d",&a,&b);adj[a].insert(b);adj[b].insert(a);cur.insert(keyOf(a,b));}
    
    vector<array<int,5>> ops;
    
    for(long long k : target){
        if(cur.count(k)) continue;
        int u = k/100000, v = k%100000;
        // BFS from u to v in current adjacency
        vector<int> par(N+1,-1);
        vector<char> vis(N+1,0);
        queue<int> q;
        q.push(u); vis[u]=1; par[u]=0;
        while(!q.empty()){
            int x=q.front();q.pop();
            if(x==v) break;
            for(int y:adj[x]) if(!vis[y]){vis[y]=1;par[y]=x;q.push(y);}
        }
        // walk back from v to u, find an edge not in target
        int ra=-1, rb=-1;
        int curN=v;
        while(curN!=u){
            int p=par[curN];
            long long ek=keyOf(curN,p);
            if(!target.count(ek)){ ra=curN; rb=p; break; }
            curN=p;
        }
        // remove (ra,rb), add (u,v)
        adj[ra].erase(rb); adj[rb].erase(ra);
        cur.erase(keyOf(ra,rb));
        adj[u].insert(v); adj[v].insert(u);
        cur.insert(k);
        ops.push_back({2,u,v,ra,rb});
    }
    
    printf("%d\n",(int)ops.size());
    for(auto &o:ops){
        printf("%d %d %d %d %d\n",o[0],o[1],o[2],o[3],o[4]);
    }
    return 0;
}
