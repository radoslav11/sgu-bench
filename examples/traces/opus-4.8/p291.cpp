#include <bits/stdc++.h>
using namespace std;

int main(){
    int q, c, n;
    long long t;
    // fast read
    if(scanf("%d %d %d %lld", &q, &c, &n, &t)!=4) return 0;
    long long N = (long long)q * c;
    const int INF = 0x3f3f3f3f;
    vector<int> key(N, INF);
    
    // effective time cap: max distance is q+c
    long long Tcap = t;
    if(Tcap > (long long)(q + c)) Tcap = q + c;
    int T = (int)Tcap;
    
    // min-heap on key
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    
    for(int i=0;i<n;i++){
        int x,y;
        scanf("%d %d",&x,&y);
        int idx = (x-1)*c + (y-1);
        // second 0, culture i -> key = i
        if(i < key[idx]){
            key[idx] = i;
            pq.push({i, idx});
        }
    }
    
    while(!pq.empty()){
        auto top = pq.top(); pq.pop();
        int k = top.first;
        int idx = top.second;
        if(k != key[idx]) continue;
        int s = k / n; // since culture index = k%n < n
        if(s >= T) continue; // can't expand further
        int newkey = k + n; // (s+1)*n + culture
        // determine coordinates
        int col = idx % c; // y-1
        int row = idx / c; // x-1
        // neighbor x-1 : row-1
        if(row > 0){
            int nidx = idx - c;
            if(newkey < key[nidx]){ key[nidx]=newkey; pq.push({newkey,nidx}); }
        }
        if(row < q-1){
            int nidx = idx + c;
            if(newkey < key[nidx]){ key[nidx]=newkey; pq.push({newkey,nidx}); }
        }
        if(col > 0){
            int nidx = idx - 1;
            if(newkey < key[nidx]){ key[nidx]=newkey; pq.push({newkey,nidx}); }
        }
        if(col < c-1){
            int nidx = idx + 1;
            if(newkey < key[nidx]){ key[nidx]=newkey; pq.push({newkey,nidx}); }
        }
    }
    
    vector<long long> cnt(n, 0);
    for(long long i=0;i<N;i++){
        if(key[i] != INF){
            cnt[key[i] % n]++;
        }
    }
    
    // output
    string out;
    out.reserve(n*4);
    char buf[24];
    for(int i=0;i<n;i++){
        sprintf(buf, "%lld\n", cnt[i]);
        out += buf;
    }
    fputs(out.c_str(), stdout);
    return 0;
}
