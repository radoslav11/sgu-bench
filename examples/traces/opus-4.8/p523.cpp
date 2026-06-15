#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,f;
    cin>>n>>f;
    vector<int> e(n);
    vector<bool> high(n,true);
    for(auto&x:e)cin>>x;
    int cur=f;
    int remaining=n;
    vector<int> result;
    while(remaining>0){
        // find first highlighted as target a
        int idx=-1;
        for(int i=0;i<n;i++)if(high[i]){idx=i;break;}
        int a=e[idx];
        // determine direction
        // collect all highlighted floors between cur and a (inclusive of a) in order of travel
        int lo=min(cur,a), hi=max(cur,a);
        vector<pair<int,int>> stops; // floor, index
        for(int i=0;i<n;i++){
            if(high[i] && e[i]>=lo && e[i]<=hi){
                stops.push_back({e[i],i});
            }
        }
        if(a>cur){
            sort(stops.begin(),stops.end());
        }else{
            sort(stops.begin(),stops.end(),greater<pair<int,int>>());
        }
        for(auto&p:stops){
            high[p.second]=false;
            remaining--;
            result.push_back(p.first);
        }
        cur=a;
    }
    for(int i=0;i<(int)result.size();i++){
        cout<<result[i]<<" ";
    }
    cout<<endl;
    return 0;
}
