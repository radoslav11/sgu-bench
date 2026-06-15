#include <bits/stdc++.h>
using namespace std;

struct Big {
    static const int BASE = 1000000000;
    vector<long long> d;
    Big(long long v=0){ if(v) d.push_back(v); }
    bool isZero() const { return d.empty(); }
    void norm(){ while(!d.empty() && d.back()==0) d.pop_back(); }
};

Big add(const Big&a, const Big&b){
    Big r; long long carry=0;
    int n=max(a.d.size(), b.d.size());
    for(int i=0;i<n||carry;i++){
        long long s=carry;
        if(i<(int)a.d.size()) s+=a.d[i];
        if(i<(int)b.d.size()) s+=b.d[i];
        r.d.push_back(s%Big::BASE);
        carry=s/Big::BASE;
    }
    r.norm();
    return r;
}

Big mulSmall(const Big&a, long long m){
    Big r;
    if(m==0||a.isZero()) return r;
    long long carry=0;
    for(int i=0;i<(int)a.d.size()||carry;i++){
        long long cur=carry;
        if(i<(int)a.d.size()) cur+=a.d[i]*m;
        r.d.push_back(cur%Big::BASE);
        carry=cur/Big::BASE;
    }
    r.norm();
    return r;
}

string toStr(const Big&a){
    if(a.isZero()) return "0";
    string s=to_string(a.d.back());
    for(int i=(int)a.d.size()-2;i>=0;i--){
        string t=to_string(a.d[i]);
        s+=string(9-t.size(),'0')+t;
    }
    return s;
}

int main(){
    int n,k;
    scanf("%d %d",&n,&k);
    vector<int> b(n);
    for(auto&x:b) scanf("%d",&x);
    sort(b.begin(), b.end());
    
    vector<Big> dp(k+1);
    dp[0]=Big(1);
    int placed=0;
    for(int i=0;i<n;i++){
        for(int j=min(k, placed+1); j>=1; j--){
            long long choices = (long long)b[i] - (j-1);
            if(choices>0){
                Big add2 = mulSmall(dp[j-1], choices);
                dp[j] = add(dp[j], add2);
            }
        }
        placed++;
    }
    
    printf("%s\n", toStr(dp[k]).c_str());
    return 0;
}
