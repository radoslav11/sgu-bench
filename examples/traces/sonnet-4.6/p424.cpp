#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin>>n;
    vector<pair<int,int>> edges;
    // Chain triangles: 1-2-3, 3-4-5, 5-6-7, ...
    // Triangle i: vertices 2i-1, 2i, 2i+1
    int v = 1;
    while(v+2<=n){
        edges.push_back({v,v+1});
        edges.push_back({v+1,v+2});
        edges.push_back({v,v+2});
        v+=2;
    }
    // if v+1<=n, connect v+1 to v
    if(v+1<=n){
        edges.push_back({v,v+1});
        v++;
    }
    cout<<v<<" "<<edges.size()<<"\n";
    for(auto&e:edges) cout<<e.first<<" "<<e.second<<"\n";
}
