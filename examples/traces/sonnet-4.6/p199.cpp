#include <bits/stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n; cin>>n;
    vector<long long> S(n),B(n);
    for(int i=0;i<n;i++) cin>>S[i]>>B[i];
    vector<int> idx(n); iota(idx.begin(),idx.end(),0);
    sort(idx.begin(),idx.end(),[&](int a,int b){ return S[a]!=S[b]?S[a]<S[b]:B[a]>B[b]; });
    vector<long long> tv; vector<int> tp,dp(n),par(n,-1);
    for(int i=0;i<n;i++){
        long long bv=B[idx[i]];
        int pos=lower_bound(tv.begin(),tv.end(),bv)-tv.begin();
        dp[i]=pos; par[i]=pos>0?tp[pos-1]:-1;
        if(pos==(int)tv.size()){tv.push_back(bv);tp.push_back(i);}
        else{tv[pos]=bv;tp[pos]=i;}
    }
    int L=tv.size(); vector<int> res; int cur=tp[L-1];
    while(cur!=-1){res.push_back(idx[cur]+1);cur=par[cur];}
    reverse(res.begin(),res.end());
    cout<<L<<"\n"; for(int x:res) cout<<x<<" "; cout<<"\n";
}
