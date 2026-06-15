#include <bits/stdc++.h>
using namespace std;
struct DSU {
    vector<int> parent, rnk;
    DSU(int n) : parent(n), rnk(n, 0) { iota(parent.begin(), parent.end(), 0); }
    int find(int x) { while(parent[x]!=x){parent[x]=parent[parent[x]];x=parent[x];}return x; }
    bool unite(int a, int b) {
        a=find(a);b=find(b);
        if(a==b)return false;
        if(rnk[a]<rnk[b])swap(a,b);
        parent[b]=a;
        if(rnk[a]==rnk[b])rnk[a]++;
        return true;
    }
};
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int M; cin>>M;
    vector<pair<long long,long long>> p1(M),p2(M),all;
    for(int i=0;i<M;i++){
        long long x1,y1,x2,y2; cin>>x1>>y1>>x2>>y2;
        p1[i]={x1,y1};p2[i]={x2,y2};
        all.push_back({x1,y1});all.push_back({x2,y2});
    }
    sort(all.begin(),all.end());
    all.erase(unique(all.begin(),all.end()),all.end());
    auto getId=[&](pair<long long,long long> p){return (int)(lower_bound(all.begin(),all.end(),p)-all.begin());};
    DSU dsu(all.size());
    for(int i=0;i<M;i++){
        int a=getId(p1[i]),b=getId(p2[i]);
        if(!dsu.unite(a,b)){cout<<i+1<<endl;return 0;}
    }
    cout<<0<<endl;
}
