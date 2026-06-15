#include <bits/stdc++.h>
using namespace std;

struct Big {
    // base 1e9, little endian
    vector<unsigned int> d;
    Big(){}
    Big(unsigned long long x){ if(x) { while(x){ d.push_back(x%1000000000ULL); x/=1000000000ULL; } } }
    bool isZero() const { return d.empty(); }
};

Big addBig(const Big&a, const Big&b){
    Big r;
    unsigned long long carry=0;
    size_t n=max(a.d.size(), b.d.size());
    for(size_t i=0;i<n||carry;i++){
        unsigned long long s=carry;
        if(i<a.d.size()) s+=a.d[i];
        if(i<b.d.size()) s+=b.d[i];
        r.d.push_back((unsigned int)(s%1000000000ULL));
        carry=s/1000000000ULL;
    }
    return r;
}

string toStr(const Big&a){
    if(a.d.empty()) return "0";
    string s=to_string(a.d.back());
    for(int i=(int)a.d.size()-2;i>=0;i--){
        string t=to_string(a.d[i]);
        s+= string(9-t.size(),'0')+t;
    }
    return s;
}

int main(){
    int n;
    if(!(cin>>n)) return 0;
    vector<int> a(n+1);
    for(int i=1;i<=n;i++) cin>>a[i];
    
    // dp[p][c] : p=m[i-1], c=m[i]
    // states indexed 0..1 for p, 0..1 for c
    Big dp[2][2];
    // init before i=1: m[0]=0, m[1] in {0,1}
    dp[0][0]=Big(1ULL);
    dp[0][1]=Big(1ULL);
    
    for(int i=1;i<=n;i++){
        Big ndp[2][2];
        for(int p=0;p<2;p++) for(int c=0;c<2;c++){
            if(dp[p][c].isZero()) continue;
            int nxt=a[i]-p-c;
            if(nxt<0||nxt>1) continue;
            if(i==n && nxt!=0) continue;
            ndp[c][nxt]=addBig(ndp[c][nxt], dp[p][c]);
        }
        for(int p=0;p<2;p++) for(int c=0;c<2;c++) dp[p][c]=ndp[p][c];
    }
    
    Big ans;
    for(int p=0;p<2;p++) for(int c=0;c<2;c++) ans=addBig(ans,dp[p][c]);
    
    cout<<toStr(ans)<<"\n";
    return 0;
}
