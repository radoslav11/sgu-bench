#include <bits/stdc++.h>
using namespace std;
int n;
vector<int> adj[16001];
int subtree_sz[16001];
int par[16001];
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> n;
    for(int i=0;i<n-1;i++){int a,b;cin>>a>>b;adj[a].push_back(b);adj[b].push_back(a);}
    if(n==1){cout<<"0 1\n1\n";return 0;}
    // iterative DFS
    vector<int> order;
    par[1]=0;
    stack<int> st;
    st.push(1);
    vector<bool> visited(n+1,false);
    visited[1]=true;
    while(!st.empty()){int u=st.top();st.pop();order.push_back(u);for(int v:adj[u])if(!visited[v]){visited[v]=true;par[v]=u;st.push(v);}}
    for(int i=0;i<=n;i++) subtree_sz[i]=1;
    for(int i=order.size()-1;i>=0;i--){int u=order[i];if(par[u]) subtree_sz[par[u]]+=subtree_sz[u];}
    int min_val=INT_MAX;
    vector<int> centroids;
    for(int k=1;k<=n;k++){int pc=n-subtree_sz[k];int mx=pc;for(int v:adj[k])if(v!=par[k]) mx=max(mx,subtree_sz[v]);if(mx<min_val){min_val=mx;centroids.clear();centroids.push_back(k);}else if(mx==min_val) centroids.push_back(k);}
    sort(centroids.begin(),centroids.end());
    cout<<min_val<<" "<<centroids.size()<<"\n";
    for(int i=0;i<(int)centroids.size();i++){if(i>0)cout<<" ";cout<<centroids[i];}
    cout<<"\n";
    return 0;
}
