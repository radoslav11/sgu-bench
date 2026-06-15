#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> adj[16001];
int subtree[16001];
int value[16001];

int main(){
    scanf("%d",&N);
    for(int i=0;i<N-1;i++){
        int a,b;
        scanf("%d %d",&a,&b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    
    // iterative DFS to compute subtree sizes
    vector<int> parent(N+1,0);
    vector<int> order;
    vector<int> stack_;
    stack_.push_back(1);
    parent[1]=0;
    vector<bool> visited(N+1,false);
    visited[1]=true;
    while(!stack_.empty()){
        int u=stack_.back();
        stack_.pop_back();
        order.push_back(u);
        for(int v:adj[u]){
            if(!visited[v]){
                visited[v]=true;
                parent[v]=u;
                stack_.push_back(v);
            }
        }
    }
    // process in reverse order
    for(int i=0;i<=N;i++) subtree[i]=1;
    for(int i=order.size()-1;i>=0;i--){
        int u=order[i];
        if(parent[u]!=0) subtree[parent[u]]+=subtree[u];
    }
    
    for(int u=1;u<=N;u++){
        int mx=0;
        for(int v:adj[u]){
            if(v==parent[u]){
                // component going up
                mx=max(mx, N - subtree[u]);
            } else {
                mx=max(mx, subtree[v]);
            }
        }
        value[u]=mx;
    }
    
    int minVal=INT_MAX;
    for(int u=1;u<=N;u++) minVal=min(minVal,value[u]);
    
    vector<int> cents;
    for(int u=1;u<=N;u++) if(value[u]==minVal) cents.push_back(u);
    
    printf("%d %d\n",minVal,(int)cents.size());
    for(int i=0;i<(int)cents.size();i++){
        if(i) printf(" ");
        printf("%d",cents[i]);
    }
    printf("\n");
    
    return 0;
}
