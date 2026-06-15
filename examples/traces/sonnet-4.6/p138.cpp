#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<int> cnt(n+1);
    for(int i=1;i<=n;i++) cin>>cnt[i];
    int G=0; for(int i=1;i<=n;i++) G+=cnt[i]; G/=2;
    cout<<G<<"\n";
    auto cmp=[](pair<int,int>a,pair<int,int>b){return a.first<b.first;};
    priority_queue<pair<int,int>,vector<pair<int,int>>,decltype(cmp)> pq(cmp);
    for(int i=1;i<=n;i++) if(cnt[i]>0) pq.push({cnt[i],i});
    auto [r1,p1]=pq.top(); pq.pop();
    auto [r2,p2]=pq.top(); pq.pop();
    cout<<p1<<" "<<p2<<"\n";
    r1--; r2--;
    if(r2>0) pq.push({r2,p2});
    int champ=p1; int champ_r=r1;
    for(int g=1;g<G;g++){
        auto [rb,pb]=pq.top(); pq.pop();
        int winner,loser,wr,lr;
        if(champ_r>=rb){winner=champ;loser=pb;wr=champ_r-1;lr=rb-1;}
        else{winner=pb;loser=champ;wr=rb-1;lr=champ_r-1;}
        cout<<winner<<" "<<loser<<"\n";
        if(lr>0) pq.push({lr,loser});
        champ=winner; champ_r=wr;
    }
}
