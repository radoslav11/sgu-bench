#include <bits/stdc++.h>
using namespace std;

int N;
long long n[4];

// enumerate one row given i and incoming horizontal bits (size i)
// produce list of (outprofile (i+1 bits), d0,d1,d2,d3)
void enumRow(int i, vector<int>& inc, vector<array<int,5>>& out){
    // sweep: triangles up(0),down(0),up(1),...,up(i). positions 0..2i
    // recursion
    function<void(int,int,int,array<int,4>&)> rec =
    [&](int p, int carry, int outprof, array<int,4>& d){
        if(p>2*i){
            out.push_back({outprof, d[0],d[1],d[2],d[3]});
            return;
        }
        if(p%2==0){
            int k=p/2;
            int left=carry;
            for(int b=0;b<2;b++) for(int r=0;r<2;r++){
                int bsum=left+b+r;
                d[bsum]++;
                int np=outprof | (b<<k);
                rec(p+1, r, np, d);
                d[bsum]--;
            }
        } else {
            int k=(p-1)/2;
            int top=inc[k];
            int left=carry;
            for(int r=0;r<2;r++){
                int bsum=top+left+r;
                d[bsum]++;
                rec(p+1, r, outprof, d);
                d[bsum]--;
            }
        }
    };
    // initial boundary carry for first up
    for(int c0=0;c0<2;c0++){
        array<int,4> d={0,0,0,0};
        rec(0, c0, 0, d);
    }
}

int main(){
    cin>>N;
    for(int j=0;j<4;j++) cin>>n[j];
    const long long CB = 456976LL; // 26^4
    auto encC=[&](int a,int b,int c,int d)->long long{
        return ((long long)a) + b*26LL + c*676LL + d*17576LL;
    };
    // dp: key = profile*CB + counts
    unordered_map<long long,long long> dp;
    dp[0*CB + encC(0,0,0,0)] = 1; // before row 0, empty profile
    
    for(int i=0;i<N;i++){
        unordered_map<long long,long long> ndp;
        // cache row enumeration by incoming profile
        unordered_map<int, vector<array<int,5>>> cache;
        for(auto &kv : dp){
            long long key=kv.first;
            long long ways=kv.second;
            long long cc = key % CB;
            int profile = (int)(key / CB);
            // decode counts
            int a0 = cc%26; cc/=26;
            int a1 = cc%26; cc/=26;
            int a2 = cc%26; cc/=26;
            int a3 = cc%26;
            // incoming bits size i = profile bits
            auto it = cache.find(profile);
            if(it==cache.end()){
                vector<int> inc(i);
                for(int k=0;k<i;k++) inc[k] = (profile>>k)&1;
                vector<array<int,5>> out;
                enumRow(i, inc, out);
                it = cache.emplace(profile, move(out)).first;
            }
            for(auto &e : it->second){
                int outprof=e[0];
                int b0=a0+e[1], b1=a1+e[2], b2=a2+e[3], b3=a3+e[4];
                if(b0>n[0]||b1>n[1]||b2>n[2]||b3>n[3]) continue;
                long long nkey = (long long)outprof*CB + encC(b0,b1,b2,b3);
                ndp[nkey]+=ways;
            }
        }
        dp.swap(ndp);
    }
    
    long long ans=0;
    long long targetCounts = encC((int)n[0],(int)n[1],(int)n[2],(int)n[3]);
    for(auto &kv : dp){
        long long cc = kv.first % CB;
        if(cc==targetCounts) ans+=kv.second;
    }
    cout<<ans<<"\n";
    return 0;
}
