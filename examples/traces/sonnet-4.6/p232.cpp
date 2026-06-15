#include <bits/stdc++.h>
using namespace std;
int main(){
    int N; long long K;
    cin >> N >> K;
    string D; cin >> D;
    K %= N;
    int LOG = 1;
    while((1<<LOG) < N) LOG++;
    LOG++;
    vector<vector<int>> rank_(LOG, vector<int>(N));
    for(int i=0;i<N;i++) rank_[0][i]=D[i]-'0';
    vector<long long> step(LOG);
    step[0]=K;
    for(int j=1;j<LOG;j++) step[j]=(step[j-1]*2)%N;
    for(int j=1;j<LOG;j++){
        map<pair<int,int>,int> mp;
        vector<pair<int,int>> keys(N);
        for(int i=0;i<N;i++) keys[i]={rank_[j-1][i], rank_[j-1][(i+step[j-1])%N]};
        vector<pair<int,int>> sorted_keys=keys;
        sort(sorted_keys.begin(),sorted_keys.end());
        sorted_keys.erase(unique(sorted_keys.begin(),sorted_keys.end()),sorted_keys.end());
        for(int i=0;i<N;i++) rank_[j][i]=lower_bound(sorted_keys.begin(),sorted_keys.end(),keys[i])-sorted_keys.begin();
    }
    int best=0;
    for(int i=1;i<N;i++) if(rank_[LOG-1][i]>rank_[LOG-1][best]) best=i;
    long long cur=best;
    for(int j=0;j<N;j++){cout<<D[cur];cur=(cur+K)%N;}
    cout<<endl;
}
