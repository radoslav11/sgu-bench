#include<bits/stdc++.h>
using namespace std;
// ... flow network code ...

int main(){
    // ... setup and compute flow ...
    
    // Flow stored as fl[u][v] = flow amount
    int fl[105][105] = {};
    for(int i=0;i<m;i++){
        int l=(edges[i].t==1)?edges[i].w:0;
        int rem=G[edges[i].u][edges[i].eidx].cap;
        int flow=l+(edges[i].w-l-rem);
        if(flow>0) fl[edges[i].u][edges[i].v]+=flow;
    }
    
    // Eliminate cycles
    // ...
    
    // Extract paths
    // ...
}
