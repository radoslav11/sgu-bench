#include <bits/stdc++.h>
using namespace std;

int main(){
    int n; 
    if(!(cin>>n)) return 0;
    vector<int> h(n);
    int total=0;
    for(auto&x:h){cin>>x; total+=x;}
    // original perimeter
    int P0 = 2*n + h[0] + h[n-1];
    for(int i=0;i+1<n;i++) P0 += abs(h[i]-h[i+1]);
    
    int Pcap = (P0+1)/2;
    int PW = Pcap+1;
    int LH = 101;
    int layerSize = LH*PW;
    const short INF = 30000;
    
    // dp[i] flat
    vector<vector<short>> dp(n+1, vector<short>(layerSize, INF));
    auto idx=[&](int p,int part){return p*PW+part;};
    
    dp[0][idx(0,0)] = 0;
    
    for(int i=1;i<=n;i++){
        int hi = h[i-1];
        auto &cur = dp[i];
        auto &prev = dp[i-1];
        for(int p=0;p<LH;p++){
            for(int part=0;part<PW;part++){
                short v = prev[idx(p,part)];
                if(v>=INF) continue;
                // remove
                int id = idx(p,part);
                if(v < cur[id]) cur[id]=v;
                // keep
                int add = 2 + (p==0 ? hi : abs(hi-p));
                int np = part+add; if(np>Pcap) np=Pcap;
                int nl = hi;
                int ns = v+hi;
                int id2 = idx(nl,np);
                if(ns < cur[id2]) cur[id2]=(short)ns;
            }
        }
    }
    
    // find best final
    int bestSum = INT_MAX, bp=-1, bpart=-1;
    for(int p=0;p<LH;p++){
        for(int part=0;part<PW;part++){
            short v = dp[n][idx(p,part)];
            if(v>=INF) continue;
            bool valid = (part==Pcap) || (2*(part+p) >= P0);
            if(valid && v < bestSum){
                bestSum=v; bp=p; bpart=part;
            }
        }
    }
    
    // reconstruct
    vector<char> kept(n+1, 0);
    int p=bp, part=bpart, s=bestSum;
    for(int i=n;i>=1;i--){
        int hi = h[i-1];
        // try removed
        if(dp[i-1][idx(p,part)]==s){
            // removed, state unchanged
            continue;
        }
        // kept: scan
        bool found=false;
        for(int pp=0;pp<LH && !found;pp++){
            for(int prevpart=0;prevpart<PW;prevpart++){
                if(dp[i-1][idx(pp,prevpart)] != s-hi) continue;
                int add = 2 + (pp==0 ? hi : abs(hi-pp));
                int np = prevpart+add; if(np>Pcap) np=Pcap;
                if(np==part){
                    kept[i]=1;
                    p=pp; part=prevpart; s-=hi;
                    found=true;
                    break;
                }
            }
        }
        // if not found, treat as removed (shouldn't happen)
    }
    
    int removedHeight = total - bestSum;
    vector<int> stolen;
    for(int i=1;i<=n;i++) if(!kept[i]) stolen.push_back(i);
    
    cout << removedHeight << "\n";
    cout << stolen.size() << "\n";
    for(size_t i=0;i<stolen.size();i++){
        cout << stolen[i];
        if(i+1<stolen.size()) cout << " ";
    }
    cout << "\n";
    return 0;
}
