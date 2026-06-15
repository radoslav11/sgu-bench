#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    scanf("%d %d", &n, &m);
    vector<vector<int>> adj(n+1);
    for(int i=0;i<m;i++){
        int a,b;
        scanf("%d %d",&a,&b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    vector<int> color(n+1, -1);
    bool ok = true;
    for(int s=1;s<=n && ok;s++){
        if(color[s]!=-1) continue;
        color[s]=0;
        queue<int> q;
        q.push(s);
        while(!q.empty()){
            int u=q.front();q.pop();
            for(int v:adj[u]){
                if(color[v]==-1){
                    color[v]=color[u]^1;
                    q.push(v);
                } else if(color[v]==color[u]){
                    ok=false;
                }
            }
        }
    }
    if(!ok){
        printf("no\n");
    } else {
        vector<int> first;
        for(int i=1;i<=n;i++){
            if(color[i]==0 || color[i]==-1){
                // uncolored (isolated) can go either day; put on first
                first.push_back(i);
            }
        }
        printf("yes\n");
        printf("%d\n", (int)first.size());
        for(int i=0;i<(int)first.size();i++){
            printf("%d%c", first[i], i+1<(int)first.size()?' ':'\n');
        }
        if(first.empty()) printf("\n");
    }
    return 0;
}
