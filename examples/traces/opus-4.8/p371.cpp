#include <bits/stdc++.h>
using namespace std;

int main(){
    long long N, M;
    if(!(cin>>N>>M)) return 0;
    long long K = M - N + 1;
    if(K < 1){ cout << "No solution\n"; return 0; }
    // find S: 3K<=S<=10K, S<=M, S >= ceil((M+2K-2)/2)
    long long lowerS = max(3*K, (M + 2*K - 1)/2); // ceil((M+2K-2)/2)
    long long upperS = min(10*K, M);
    if(lowerS > upperS){ cout << "No solution\n"; return 0; }
    long long S = lowerS;
    long long R = M - S;
    // distribute c_i in [3,10]
    vector<long long> c(K, 3);
    long long extra = S - 3*K;
    for(long long i=0;i<K && extra>0;i++){
        long long add = min(extra, 7LL);
        c[i]+=add; extra-=add;
    }
    // build circles
    vector<vector<long long>> circles(K);
    long long nextId = 1;
    long long transferStation = 0;
    set<long long> transferSet;
    for(long long i=0;i<K;i++){
        vector<long long> &lst = circles[i];
        if(transferStation!=0){ lst.push_back(transferStation); transferSet.insert(transferStation);}
        while((long long)lst.size() < c[i]){
            lst.push_back(nextId++);
        }
        if(i+1<K){
            transferStation = lst.back();
            transferSet.insert(transferStation);
        }
    }
    // collect free (non-transfer) circle stations
    vector<long long> freeStations;
    for(long long i=0;i<K;i++){
        for(long long st: circles[i]){
            if(transferSet.find(st)==transferSet.end()){
                freeStations.push_back(st);
            }
        }
    }
    // attach R radials
    vector<pair<long long,long long>> radials;
    for(long long r=0;r<R;r++){
        long long term = nextId++;
        long long connect = freeStations[r];
        radials.push_back({term, connect});
    }
    // output
    cout << K << "\n";
    for(long long i=0;i<K;i++){
        cout << circles[i].size();
        for(long long st: circles[i]) cout << " " << st;
        cout << "\n";
    }
    cout << R << "\n";
    for(auto &pr: radials){
        cout << pr.first << " " << pr.second << "\n";
    }
    return 0;
}
