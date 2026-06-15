#include <bits/stdc++.h>
using namespace std;

int main(){
    int n,m;
    scanf("%d %d",&n,&m);
    vector<int> par(n+1,0);
    vector<vector<int>> children(n+1);
    for(int i=2;i<=n;i++){
        scanf("%d",&par[i]);
        children[par[i]].push_back(i);
    }
    int firstLeaf = n-m+1;
    vector<long long> val(n+1,0);
    for(int i=firstLeaf;i<=n;i++){
        long long v; scanf("%lld",&v); val[i]=v;
    }
    const long long INF = 2147483647LL;
    vector<set<long long>*> nodeSet(n+1,nullptr);
    vector<long long> ans(n+1, INF);
    vector<int> order;
    {
        vector<int> stk; stk.push_back(1);
        while(!stk.empty()){
            int u=stk.back(); stk.pop_back();
            order.push_back(u);
            for(int c: children[u]) stk.push_back(c);
        }
        reverse(order.begin(), order.end());
    }
    for(int u: order){
        if(u>=firstLeaf){
            nodeSet[u]=new set<long long>();
            nodeSet[u]->insert(val[u]);
            ans[u]=INF;
            continue;
        }
        set<long long>* cur=nullptr;
        long long best=INF;
        for(int c: children[u]){
            if(ans[c]<best) best=ans[c];
            set<long long>* s = nodeSet[c];
            if(cur==nullptr){ cur=s; nodeSet[c]=nullptr; continue; }
            if(cur->size() < s->size()) swap(cur,s);
            for(long long x: *s){
                auto it = cur->lower_bound(x);
                if(it!=cur->end()){ long long d=*it-x; if(d<best) best=d; }
                if(it!=cur->begin()){ auto pit=prev(it); long long d=x-*pit; if(d<best) best=d; }
                cur->insert(x);
            }
            delete s;
            nodeSet[c]=nullptr;
        }
        nodeSet[u]=cur;
        ans[u]=best;
    }
    for(int i=1;i<=n-m;i++){
        printf("%lld",ans[i]);
        if(i<n-m) printf(" ");
    }
    printf("\n");
    return 0;
}
