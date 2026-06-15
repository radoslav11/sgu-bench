#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M;
    cin>>N>>M;
    int K=M-N+1;
    if(K<1){cout<<"No solution";return 0;}
    int lo=3*K, hi=min({10*K, M, N+K-1});
    if(lo>hi){cout<<"No solution";return 0;}
    int S=lo; int R=M-S;
    vector<int> sz(K,3);
    vector<vector<int>> circles(K);
    int cur=1;
    for(int i=0;i<K;i++){
        circles[i].resize(sz[i]);
        if(i==0){for(int j=0;j<sz[i];j++)circles[i][j]=cur++;}
        else{circles[i][0]=circles[i-1][sz[i-1]-1];for(int j=1;j<sz[i];j++)circles[i][j]=cur++;}
    }
    set<int> sharedSt;
    for(int i=0;i<K-1;i++)sharedSt.insert(circles[i][sz[i]-1]);
    vector<int> avail;
    for(int i=0;i<K;i++)for(int s:circles[i])if(!sharedSt.count(s))avail.push_back(s);
    if((int)avail.size()<R){cout<<"No solution";return 0;}
    vector<pair<int,int>> radials;
    for(int i=0;i<R;i++)radials.push_back({avail[i],cur++});
    cout<<K<<"\n";
    for(int i=0;i<K;i++){cout<<sz[i];for(int s:circles[i])cout<<" "<<s;cout<<"\n";}
    cout<<R<<"\n";
    for(auto&p:radials)cout<<p.first<<" "<<p.second<<"\n";
}
