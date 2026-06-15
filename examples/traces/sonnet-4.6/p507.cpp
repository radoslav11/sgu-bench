#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m;scanf("%d %d",&n,&m);
    vector<vector<int>> ch(n+1);
    for(int i=2;i<=n;i++){int p;scanf("%d",&p);ch[p].push_back(i);}
    int internal=n-m;
    vector<long long> ans(n+1,2e18);
    vector<int> lv(n+1,0);
    for(int i=n-m+1;i<=n;i++)scanf("%d",&lv[i]);
    vector<set<int>*> S(n+1,nullptr);
    vector<int> order;
    vector<int> stk={1};
    while(!stk.empty()){int v=stk.back();stk.pop_back();order.push_back(v);for(int c:ch[v])stk.push_back(c);}
    reverse(order.begin(),order.end());
    const long long INF=2147483647LL;
    for(int v:order){
        if(v>internal){S[v]=new set<int>();S[v]->insert(lv[v]);ans[v]=INF;}
        else{
            int big=-1;
            for(int c:ch[v])if(big==-1||S[c]->size()>S[big]->size())big=c;
            ans[v]=INF;
            for(int c:ch[v])ans[v]=min(ans[v],ans[c]);
            S[v]=S[big];
            for(int c:ch[v]){
                if(c==big)continue;
                for(int x:*S[c]){
                    auto it=S[v]->insert(x).first;
                    auto nx=next(it);
                    if(nx!=S[v]->end())ans[v]=min(ans[v],(long long)(*nx-x));
                    if(it!=S[v]->begin()){auto pv=prev(it);ans[v]=min(ans[v],(long long)(x-*pv));}
                }
                delete S[c];S[c]=nullptr;
            }
        }
    }
    for(int v=1;v<=internal;v++){printf("%lld",ans[v]>=(long long)2e18?INF:ans[v]);if(v<internal)printf(" ");}
    printf("\n");
    if(S[1])delete S[1];
}
