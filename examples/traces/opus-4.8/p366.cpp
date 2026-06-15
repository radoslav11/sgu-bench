#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, K;
    scanf("%d %d", &N, &K);
    // delta = a-b in [-50,50], index = delta+50 (0..100)
    vector<vector<pair<int,int>>> byDelta(101); // (w=a+b, origIndex 1-based)
    for(int i=0;i<N;i++){
        int a,b; scanf("%d %d",&a,&b);
        int delta=a-b;
        int w=a+b;
        byDelta[delta+50].push_back({w, i+1});
    }
    // keep top K per delta by w
    struct Item{int delta; int w; int idx;};
    vector<Item> items;
    for(int di=0; di<101; di++){
        auto &v = byDelta[di];
        if(v.empty()) continue;
        sort(v.begin(), v.end(), [](const pair<int,int>&x, const pair<int,int>&y){
            return x.first > y.first;
        });
        int lim = min((int)v.size(), K);
        int delta = di-50;
        for(int j=0;j<lim;j++){
            items.push_back({delta, v[j].first, v[j].second});
        }
    }
    int M = items.size();
    const int D = 2001; // diff value -1000..1000 -> index +1000
    const int OFF = 1000;
    // dp[k][dIdx] = max value (A+B), -1 unreachable
    vector<vector<int>> dp(K+1, vector<int>(D, -1));
    dp[0][OFF] = 0;
    
    int states = (K+1)*D;
    int words = (states + 63)/64;
    // choice[i] bitset over states
    vector<vector<uint64_t>> choice(M, vector<uint64_t>(words, 0ULL));
    
    for(int i=0;i<M;i++){
        int delta = items[i].delta;
        int w = items[i].w;
        for(int k=K;k>=1;k--){
            // prev layer k-1
            auto &prev = dp[k-1];
            auto &cur = dp[k];
            for(int pdIdx=0; pdIdx<D; pdIdx++){
                if(prev[pdIdx] < 0) continue;
                int ndIdx = pdIdx + delta;
                if(ndIdx < 0 || ndIdx >= D) continue;
                int nv = prev[pdIdx] + w;
                if(nv > cur[ndIdx]){
                    cur[ndIdx] = nv;
                    int state = k*D + ndIdx;
                    choice[i][state>>6] |= (1ULL << (state & 63));
                }
            }
        }
    }
    
    // find best final state at k=K
    int bestDIdx = -1;
    int bestAbs = INT_MAX;
    int bestVal = -1;
    for(int dIdx=0; dIdx<D; dIdx++){
        if(dp[K][dIdx] < 0) continue;
        int ab = abs(dIdx - OFF);
        int val = dp[K][dIdx];
        if(ab < bestAbs || (ab==bestAbs && val>bestVal)){
            bestAbs = ab;
            bestVal = val;
            bestDIdx = dIdx;
        }
    }
    
    int value = dp[K][bestDIdx];
    int diff = bestDIdx - OFF;
    int A = (value + diff)/2;
    int B = (value - diff)/2;
    
    // reconstruct
    vector<int> res;
    int k = K;
    int dIdx = bestDIdx;
    int i = M-1;
    while(k>0){
        int state = k*D + dIdx;
        while(i>=0 && !((choice[i][state>>6] >> (state & 63)) & 1ULL)){
            i--;
        }
        // item i used
        res.push_back(items[i].idx);
        int delta = items[i].delta;
        dIdx -= delta;
        k--;
        i--;
    }
    
    sort(res.begin(), res.end());
    printf("%d %d\n", A, B);
    for(size_t j=0;j<res.size();j++){
        if(j) printf(" ");
        printf("%d", res[j]);
    }
    printf("\n");
    return 0;
}
