#include <bits/stdc++.h>
using namespace std;
int par[200005];
int find(int x){return par[x]==x?x:par[x]=find(par[x]);}
int main(){
    int N,M,Q;cin>>N>>M>>Q;
    vector<vector<int>>children(N+M+5);
    vector<int>minLeaf(N+M+5),nodeId(N+M+5);
    for(int i=1;i<=N;i++){par[i]=i;minLeaf[i]=i;nodeId[i]=i;}
    int nextNode=N+1;
    vector<tuple<int,int,vector<int>>>splits(M);
    for(int i=0;i<M;i++){int Y,L;cin>>Y>>L;vector<int>V(L);for(int j=0;j<L;j++)cin>>V[j];splits[i]={Y,L,V};}
    sort(splits.begin(),splits.end(),[](auto&a,auto&b){return get<0>(a)>get<0>(b);});
    for(auto&[Y,L,V]:splits){
        int nn=nextNode++;par[nn]=nn;nodeId[nn]=nn;minLeaf[nn]=INT_MAX;
        for(int v:V){int r=find(v);children[nn].push_back(nodeId[r]);par[r]=nn;}
        for(int c:children[nn])minLeaf[nn]=min(minLeaf[nn],minLeaf[c]);
        sort(children[nn].begin(),children[nn].end(),[&](int a,int b){return minLeaf[a]<minLeaf[b];});
    }
    vector<int>order,roots;
    for(int i=1;i<nextNode;i++)if(find(i)==i)roots.push_back(i);
    sort(roots.begin(),roots.end(),[&](int a,int b){return minLeaf[a]<minLeaf[b];});
    function<void(int)>dfs=[&](int u){if(u<=N){order.push_back(u);return;}for(int c:children[u])dfs(c);};
    for(int r:roots)dfs(r);
    for(int i=0;i<Q;i++){int q;cin>>q;cout<<order[q-1]<<"\n";}
}
