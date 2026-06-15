#include <bits/stdc++.h>
using namespace std;

int parent[2005], rnk_[2005];
int N;

inline int findp(int x){
    while(parent[x]!=x){ parent[x]=parent[parent[x]]; x=parent[x]; }
    return x;
}
inline bool uni(int a,int b){
    a=findp(a); b=findp(b);
    if(a==b) return false;
    if(rnk_[a]<rnk_[b]) swap(a,b);
    parent[b]=a;
    if(rnk_[a]==rnk_[b]) rnk_[a]++;
    return true;
}
inline void resetDSU(){
    for(int i=1;i<=N;i++){ parent[i]=i; rnk_[i]=0; }
}

int main(){
    int M,K;
    if(scanf("%d %d %d",&N,&M,&K)!=3) return 0;
    vector<int> ea(K),eb(K),ec(K),ep(K);
    for(int i=0;i<K;i++){
        scanf("%d %d %d %d",&ea[i],&eb[i],&ec[i],&ep[i]);
    }
    // base MST
    vector<int> ord(K);
    for(int i=0;i<K;i++) ord[i]=i;
    sort(ord.begin(),ord.end(),[&](int x,int y){ return ep[x]<ep[y]; });
    resetDSU();
    vector<int> mstEdges; // indices into edge arrays, in sorted(increasing weight) order
    mstEdges.reserve(N>0?N:1);
    for(int idx: ord){
        if(uni(ea[idx],eb[idx])){
            mstEdges.push_back(idx);
        }
    }
    // group edges by airline
    vector<vector<int>> byAirline(M+1);
    for(int i=0;i<K;i++) byAirline[ec[i]].push_back(i);
    
    long long bestCost = LLONG_MAX;
    int bestR = 1;
    
    for(int R=1;R<=M;R++){
        resetDSU();
        // union R's free edges
        for(int e: byAirline[R]){
            uni(ea[e],eb[e]);
        }
        long long cost=0;
        for(int e: mstEdges){
            if(findp(ea[e])!=findp(eb[e])){
                uni(ea[e],eb[e]);
                if(ec[e]!=R) cost+=ep[e];
            }
        }
        if(cost<bestCost){
            bestCost=cost;
            bestR=R;
        }
    }
    
    // reconstruct bought list for bestR
    int R=bestR;
    resetDSU();
    for(int e: byAirline[R]){
        uni(ea[e],eb[e]);
    }
    vector<int> bought;
    for(int e: mstEdges){
        if(findp(ea[e])!=findp(eb[e])){
            uni(ea[e],eb[e]);
            if(ec[e]!=R) bought.push_back(e+1); // 1-based flight index
        }
    }
    
    // output
    printf("%lld %d %d\n", bestCost, bestR, (int)bought.size());
    for(int id: bought) printf("%d\n", id);
    
    return 0;
}
