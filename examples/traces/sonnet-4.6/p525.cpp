#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    cin >> n >> m;
    vector<int> A(m), B(m);
    vector<vector<int>> g(n+1), rg(n+1);
    for(int i=0;i<m;i++){
        cin>>A[i]>>B[i];
        g[A[i]].push_back(B[i]);
        rg[B[i]].push_back(A[i]);
    }
    // Kosaraju's SCC
    vector<int> order, comp(n+1,-1);
    vector<bool> vis(n+1,false);
    function<void(int)> dfs1=[&](int u){vis[u]=true;for(int v:g[u])if(!vis[v])dfs1(v);order.push_back(u);};
    for(int i=1;i<=n;i++)if(!vis[i])dfs1(i);
    int ns=0;
    function<void(int,int)> dfs2=[&](int u,int c){comp[u]=c;for(int v:rg[u])if(comp[v]==-1)dfs2(v,c);};
    for(int i=n-1;i>=0;i--){int u=order[i];if(comp[u]==-1)dfs2(u,ns++);}
    vector<int> sz(ns,0);
    for(int i=1;i<=n;i++)sz[comp[i]]++;
    // Reachability in condensation
    vector<vector<bool>> reach(ns,vector<bool>(ns,false));
    vector<vector<int>> cg(ns);
    for(int u=1;u<=n;u++)for(int v:g[u])if(comp[u]!=comp[v])cg[comp[u]].push_back(comp[v]);
    for(int s=0;s<ns;s++){queue<int>q;q.push(s);reach[s][s]=true;while(!q.empty()){int u=q.front();q.pop();for(int v:cg[u])if(!reach[s][v]){reach[s][v]=true;q.push(v);}}}
    int base=*max_element(sz.begin(),sz.end());
    int w=base;
    for(int i=0;i<m;i++){int ca=comp[A[i]],cb=comp[B[i]];if(ca==cb)continue;int tot=0;for(int s=0;s<ns;s++)if(reach[ca][s]&&reach[s][cb])tot+=sz[s];w=max(w,tot);}
    vector<int> res;
    for(int i=0;i<m;i++){int ca=comp[A[i]],cb=comp[B[i]];if(ca==cb){if(sz[ca]==w)res.push_back(i+1);continue;}int tot=0;for(int s=0;s<ns;s++)if(reach[ca][s]&&reach[s][cb])tot+=sz[s];if(tot==w)res.push_back(i+1);}
    cout<<w<<"\n"<<res.size()<<"\n";
    for(int x:res)cout<<x<<" ";cout<<"\n";
}
