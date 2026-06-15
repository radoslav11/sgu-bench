#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    scanf("%d %d", &n, &m);
    vector<vector<int>> g(n);
    vector<int> ra(m), rb(m);
    for(int i=0;i<m;i++){
        int a,b; scanf("%d %d",&a,&b); a--; b--;
        ra[i]=a; rb[i]=b;
        g[a].push_back(b);
    }
    // Tarjan iterative
    vector<int> num(n,-1), low(n,0), comp(n,-1);
    vector<bool> onstk(n,false);
    vector<int> stk;
    int idx=0, nc=0;
    for(int s=0;s<n;s++){
        if(num[s]!=-1) continue;
        // stack of (node, child index)
        vector<pair<int,int>> st;
        st.push_back({s,0});
        while(!st.empty()){
            int u=st.back().first;
            int &pi=st.back().second;
            if(pi==0){
                num[u]=low[u]=idx++;
                stk.push_back(u); onstk[u]=true;
            }
            if(pi < (int)g[u].size()){
                int v=g[u][pi]; pi++;
                if(num[v]==-1){
                    st.push_back({v,0});
                } else if(onstk[v]){
                    low[u]=min(low[u], num[v]);
                }
            } else {
                if(low[u]==num[u]){
                    while(true){
                        int w=stk.back(); stk.pop_back(); onstk[w]=false;
                        comp[w]=nc;
                        if(w==u) break;
                    }
                    nc++;
                }
                st.pop_back();
                if(!st.empty()){
                    int p=st.back().first;
                    low[p]=min(low[p], low[u]);
                }
            }
        }
    }
    // comp ids: sinks have small ids. edge ca->cb (distinct) => ca>cb
    vector<int> compSize(nc,0);
    for(int v=0;v<n;v++) compSize[comp[v]]++;
    int S0=0;
    for(int c=0;c<nc;c++) S0=max(S0,compSize[c]);
    
    // condensation edges
    vector<pair<int,int>> cedges;
    for(int i=0;i<m;i++){
        int ca=comp[ra[i]], cb=comp[rb[i]];
        if(ca!=cb) cedges.push_back({ca,cb});
    }
    sort(cedges.begin(), cedges.end());
    cedges.erase(unique(cedges.begin(),cedges.end()), cedges.end());
    
    vector<vector<int>> fadj(nc), radj(nc);
    for(auto&e:cedges){
        fadj[e.first].push_back(e.second);
        radj[e.second].push_back(e.first);
    }
    
    // reachFrom: process increasing id (sinks first), since edge ca->cb has cb<ca
    vector<bitset<1000>> reachFrom(nc), reachTo(nc);
    for(int c=0;c<nc;c++){
        reachFrom[c].set(c);
        for(int d: fadj[c]){
            reachFrom[c]|=reachFrom[d];
        }
    }
    // reachTo: who can reach c. process decreasing id (sources have large id)
    for(int c=nc-1;c>=0;c--){
        reachTo[c].set(c);
        for(int p: radj[c]){
            reachTo[c]|=reachTo[p];
        }
    }
    
    // for each road compute achievable
    vector<int> ach(m);
    int w=S0;
    if(m==0) w=S0;
    for(int i=0;i<m;i++){
        int A=comp[ra[i]], B=comp[rb[i]];
        if(A==B){
            ach[i]=S0;
        } else {
            bitset<1000> inter = reachFrom[A] & reachTo[B];
            int sum=0;
            for(int b=inter._Find_first(); b<1000; b=inter._Find_next(b)){
                sum+=compSize[b];
            }
            ach[i]=max(S0, sum);
        }
        w=max(w, ach[i]);
    }
    
    vector<int> good;
    for(int i=0;i<m;i++){
        if(ach[i]==w) good.push_back(i+1);
    }
    
    printf("%d\n", w);
    printf("%d\n", (int)good.size());
    for(size_t i=0;i<good.size();i++){
        printf("%d%c", good[i], i+1<good.size()?' ':'\n');
    }
    if(good.empty()) printf("\n");
    return 0;
}
