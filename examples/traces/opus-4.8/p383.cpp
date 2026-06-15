#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    if(scanf("%d",&n)!=1) return 0;
    vector<long long> x(n), y(n);
    for(int i=0;i<n;i++) scanf("%lld %lld",&x[i],&y[i]);
    
    vector<double> minDist(n, 1e30);
    vector<int> parent(n, -1);
    vector<char> inMST(n,0);
    minDist[0]=0;
    vector<vector<pair<int,double>>> adj(n);
    for(int it=0; it<n; it++){
        int u=-1; double best=1e30;
        for(int v=0;v<n;v++) if(!inMST[v] && minDist[v]<best){best=minDist[v];u=v;}
        if(u==-1) break;
        inMST[u]=1;
        if(parent[u]!=-1){
            double w = sqrt((double)((x[u]-x[parent[u]])*(x[u]-x[parent[u]]) + (y[u]-y[parent[u]])*(y[u]-y[parent[u]])));
            adj[u].push_back({parent[u], w});
            adj[parent[u]].push_back({u, w});
        }
        for(int v=0;v<n;v++) if(!inMST[v]){
            double d = (double)((x[u]-x[v])*(x[u]-x[v]) + (y[u]-y[v])*(y[u]-y[v]));
            if(d < minDist[v]){ minDist[v]=d; parent[v]=u; }
        }
    }
    
    int LOG=1;
    while((1<<LOG) < n) LOG++;
    LOG++;
    vector<vector<int>> up(LOG, vector<int>(n, -1));
    vector<vector<double>> mx(LOG, vector<double>(n, 0.0));
    vector<int> depth(n, 0);
    
    vector<char> vis(n,0);
    {
        stack<int> st;
        st.push(0); vis[0]=1; up[0][0]=-1; depth[0]=0;
        while(!st.empty()){
            int u=st.top(); st.pop();
            for(auto &e: adj[u]){
                int v=e.first;
                if(!vis[v]){
                    vis[v]=1;
                    up[0][v]=u;
                    mx[0][v]=e.second;
                    depth[v]=depth[u]+1;
                    st.push(v);
                }
            }
        }
    }
    
    for(int k=1;k<LOG;k++){
        for(int v=0;v<n;v++){
            int mid=up[k-1][v];
            if(mid==-1){ up[k][v]=-1; mx[k][v]=mx[k-1][v]; }
            else {
                up[k][v]=up[k-1][mid];
                mx[k][v]=max(mx[k-1][v], mx[k-1][mid]);
            }
        }
    }
    
    int q;
    scanf("%d",&q);
    while(q--){
        int s,t;
        scanf("%d %d",&s,&t);
        s--; t--;
        if(s==t){ printf("%.10f\n", 0.0); continue; }
        double ans=0;
        int a=s,b=t;
        if(depth[a]<depth[b]) swap(a,b);
        int diff=depth[a]-depth[b];
        for(int k=0;k<LOG;k++){
            if(diff & (1<<k)){
                ans=max(ans, mx[k][a]);
                a=up[k][a];
            }
        }
        if(a!=b){
            for(int k=LOG-1;k>=0;k--){
                if(up[k][a]!=up[k][b]){
                    ans=max(ans, mx[k][a]);
                    ans=max(ans, mx[k][b]);
                    a=up[k][a];
                    b=up[k][b];
                }
            }
            ans=max(ans, mx[0][a]);
            ans=max(ans, mx[0][b]);
        }
        printf("%.10f\n", ans);
    }
    
    return 0;
}
