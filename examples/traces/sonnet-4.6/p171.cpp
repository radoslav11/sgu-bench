#include <bits/stdc++.h>
using namespace std;
int main(){
    int K; cin>>K;
    vector<int> N(K),Q(K);
    for(int i=0;i<K;i++) cin>>N[i];
    for(int i=0;i<K;i++) cin>>Q[i];
    int total=0; for(int i=0;i<K;i++) total+=N[i];
    vector<int> P(total),w(total);
    for(int i=0;i<total;i++) cin>>P[i];
    for(int i=0;i<total;i++) cin>>w[i];
    vector<int> zoneOrder(K); iota(zoneOrder.begin(),zoneOrder.end(),0);
    sort(zoneOrder.begin(),zoneOrder.end(),[&](int a,int b){return Q[a]>Q[b];});
    vector<int> ans(total);
    set<tuple<int,int,int>> st;
    for(int i=0;i<total;i++) st.insert({P[i],w[i],i});
    for(int zi=0;zi<K;zi++){
        int z=zoneOrder[zi]; int need=N[z];
        vector<int> chosen;
        auto it=st.begin();
        while((int)chosen.size()<need && it!=st.end() && get<0>(*it)<=Q[z]){
            chosen.push_back(get<2>(*it)); ++it;
        }
        if((int)chosen.size()<need){
            vector<pair<int,int>> rem;
            for(auto&x:st) if(get<0>(x)>Q[z]) rem.push_back({get<1>(x),get<2>(x)});
            sort(rem.begin(),rem.end());
            for(auto&r:rem){ if((int)chosen.size()>=need) break; chosen.push_back(r.second); }
        }
        for(int s:chosen){ ans[s]=z+1; st.erase({P[s],w[s],s}); }
    }
    for(int i=0;i<total;i++) cout<<ans[i]<<(i+1<total?" ":"\n");
}
