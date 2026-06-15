#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m; scanf("%d %d",&n,&m);
    vector<vector<pair<int,int>>> adj(n+1);
    for(int i=0;i<m;i++){int x,y,c;scanf("%d %d %d",&x,&y,&c);adj[x].push_back({y,-c});}
    for(int i=1;i<=n;i++) adj[0].push_back({i,10000});
    vector<int> dist(n+1,0);
    for(int i=1;i<=n;i++) dist[i]=10000;
    dist[0]=0;
    vector<int> cnt(n+1,0);
    vector<char> inq(n+1,0);
    deque<int> q; q.push_back(0); inq[0]=1;
    bool neg=false;
    while(!q.empty()){
        int u=q.front();q.pop_front();inq[u]=0;
        for(auto&e:adj[u]){int v=e.first,w=e.second;
            if(dist[u]+w<dist[v]){dist[v]=dist[u]+w;
                if(!inq[v]){inq[v]=1;
                    if(++cnt[v]>n+1){neg=true;break;}
                    if(!q.empty()&&dist[v]<dist[q.front()])q.push_front(v);else q.push_back(v);
                }
            }
        }
        if(neg)break;
    }
    if(neg){printf("-1\n");return 0;}
    for(int i=1;i<=n;i++){
        if(dist[i]<-10000){printf("-1\n");return 0;}
        printf("%d ",dist[i]);
    }
    printf("\n");
    return 0;
}
