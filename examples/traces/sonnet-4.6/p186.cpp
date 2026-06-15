#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<int> L(n);
    for(auto &x:L) cin>>x;
    sort(L.begin(),L.end());
    if(n==1){cout<<0;return 0;}
    for(int k=0;;k++){
        // open k links greedily from smallest chains
        // count how many chain-segments remain after opening k links
        // opening a link from chain of len l: if l==1, chain disappears, else chain shrinks
        // each opened link can join 2 segments (used as connector)
        // segments start = n, each opened link reduces segments by 1 (net: use connector to join)
        // but opening from middle splits... assume always open end links
        // after k operations: segments = n + (extra splits) - k_joins
        // if we open end links only: no extra splits
        // remaining segments after using k connectors on n segments: n - k
        // but we need to account for chains fully consumed
        vector<int> chains = L;
        int segs = n;
        int ops = 0;
        int connectors = 0;
        // simulate: open k links from smallest available end-links
        // each op: take from smallest chain
        vector<int> c = L;
        int total_segs = n;
        int conn = 0;
        for(int i=0;i<k;i++){
            // open from smallest chain
            if(c[0]>0){
                c[0]--;
                conn++;
                if(c[0]==0){
                    total_segs--;
                }
                sort(c.begin(),c.end());
            }
        }
        // now total_segs segments, conn connectors
        // need total_segs-1 connectors to join all
        if(conn >= total_segs-1){
            cout<<k;
            return 0;
        }
    }
}
