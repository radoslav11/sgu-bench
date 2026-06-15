#include <bits/stdc++.h>
using namespace std;

int n, k;

int main(){
    cin>>n>>k;
    int full=1<<n;
    map<pair<int,int>, vector<long long>> cur;
    {
        vector<long long> v(k+1,0);
        v[0]=1;
        cur[{0,0}]=v;
    }
    for(int row=0; row<n; row++){
        map<pair<int,int>, vector<long long>> nxt;
        for(auto &e: cur){
            int pp=e.first.first;
            int p=e.first.second;
            vector<long long>& cnt=e.second;
            for(int m=0;m<full;m++){
                bool ok=true;
                if( ((m<<2)&p) || ((m>>2)&p) ) ok=false;
                if(ok && ( ((m<<1)&pp) || ((m>>1)&pp) )) ok=false;
                if(!ok) continue;
                int pc=__builtin_popcount(m);
                if(pc>k) continue;
                auto &target=nxt[{p,m}];
                if(target.empty()) target.assign(k+1,0);
                for(int c=0;c+pc<=k;c++){
                    if(cnt[c]) target[c+pc]+=cnt[c];
                }
            }
        }
        cur=move(nxt);
    }
    long long ans=0;
    for(auto &e:cur){
        ans+=e.second[k];
    }
    cout<<ans<<endl;
    return 0;
}
