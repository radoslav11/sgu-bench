#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; if(scanf("%d",&n)!=1)return 0;
    int virt=n+1;
    vector<pair<int,int>> edges;
    vector<vector<int>> g(n+2);
    for(int i=1;i<=n;i++){
        int x;
        while(scanf("%d",&x)==1 && x!=0){
            if(i<x){int id=edges.size();edges.push_back({i,x});g[i].push_back(id);g[x].push_back(id);}
            else if(i>x){
                for(int id=0;id<(int)edges.size();id++) if(edges[id]==make_pair(x,i)&&find(g[i].begin(),g[i].end(),id)==g[i].end()){g[i].push_back(id);break;}
            }
        }
    }
    for(int i=1;i<=n;i++) if(g[i].size()%2==1){int id=edges.size();edges.push_back({i,virt});g[i].push_back(id);g[virt].push_back(id);}
    vector<int> color(edges.size(),0),ptr(n+2,0);
    vector<bool> used(edges.size(),false);
    for(int s=1;s<=virt;s++){
        vector<int> vstk={s},edgePath,circuit;
        while(!vstk.empty()){
            int v=vstk.back();
            while(ptr[v]<(int)g[v].size()&&used[g[v][ptr[v]]])ptr[v]++;
            if(ptr[v]==(int)g[v].size()){vstk.pop_back();if(!edgePath.empty()){circuit.push_back(edgePath.back());edgePath.pop_back();}}
            else{int id=g[v][ptr[v]];used[id]=true;int u=edges[id].first==v?edges[id].second:edges[id].first;vstk.push_back(u);edgePath.push_back(id);}
        }
        int c=1;for(int id:circuit){color[id]=c;c=3-c;}
    }
    bool ok=true;
    for(int i=1;i<=n;i++){int rc=0;bool w=false,b=false;for(int id:g[i]){if(edges[id].first==virt||edges[id].second==virt)continue;rc++;if(color[id]==1)w=true;else b=true;}if(rc>=2&&(!w||!b))ok=false;}
    if(!ok){printf("No solution\n");return 0;}
    for(int i=1;i<=n;i++){for(int id:g[i]){if(edges[id].first==virt||edges[id].second==virt)continue;printf("%d ",color[id]);}printf("0\n");}
    return 0;
}
