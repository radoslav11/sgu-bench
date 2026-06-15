#include <bits/stdc++.h>
using namespace std;
typedef vector<uint32_t> Big;
const uint32_t BASE=1000000000u;
void addTo(Big&a,const Big&b){
    uint64_t carry=0;
    for(size_t i=0;i<b.size()||carry;i++){
        if(i==a.size())a.push_back(0);
        uint64_t cur=(uint64_t)a[i]+carry+(i<b.size()?b[i]:0);
        a[i]=cur%BASE; carry=cur/BASE;
    }
}
string toStr(const Big&a){
    if(a.empty())return "0";
    string s=to_string(a.back());
    for(int i=(int)a.size()-2;i>=0;i--){
        string t=to_string(a[i]);
        s+=string(9-t.size(),'0')+t;
    }
    return s;
}
int main(){
    string sigma; cin>>sigma;
    int A=sigma.size();
    int K; cin>>K;
    int S; cin>>S;
    int L; cin>>L;
    vector<int> term(K+1,0);
    for(int i=0;i<L;i++){int t;cin>>t;term[t]=1;}
    vector<vector<int>> phi(K+1,vector<int>(A));
    for(int i=1;i<=K;i++)for(int j=0;j<A;j++)cin>>phi[i][j];
    vector<vector<int>> chi(K+1,vector<int>(A));
    for(int i=1;i<=K;i++)for(int j=0;j<A;j++)cin>>chi[i][j];
    int N; cin>>N;
    vector<vector<int>> nxt(K+1,vector<int>(A,0));
    for(int u=1;u<=K;u++)for(int c=0;c<A;c++){
        vector<char> vis(K+1,0);
        int cur=u; int res=0;
        while(true){
            if(vis[cur]){res=0;break;}
            vis[cur]=1;
            if(chi[cur][c]==0){res=phi[cur][c];break;}
            cur=phi[cur][c];
        }
        nxt[u][c]=res;
    }
    vector<Big> dp(K+1), ndp(K+1);
    for(int u=1;u<=K;u++) if(term[u]) dp[u]=Big{1};
    for(int i=1;i<=N;i++){
        for(int u=1;u<=K;u++)ndp[u].clear();
        for(int u=1;u<=K;u++){
            for(int c=0;c<A;c++){
                int v=nxt[u][c];
                if(v!=0 && !dp[v].empty()) addTo(ndp[u],dp[v]);
            }
        }
        dp.swap(ndp);
    }
    cout<<toStr(dp[S])<<"\n";
}
