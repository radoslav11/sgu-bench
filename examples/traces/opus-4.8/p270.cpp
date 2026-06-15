#include<bits/stdc++.h>
using namespace std;
int N,M;
vector<pair<int,int>> sw;
set<int> res;
set<pair<int,vector<int>>> memo;
void solve(int pos, vector<int> rem){
    bool any=false;
    for(int i=0;i<(int)rem.size();i++){
        int idx=rem[i];
        if(sw[idx].first==pos||sw[idx].second==pos){any=true;break;}
    }
    if(!any){res.insert(pos);return;}
    auto key=make_pair(pos,rem);
    if(memo.count(key))return;
    memo.insert(key);
    for(int i=0;i<(int)rem.size();i++){
        int idx=rem[i];
        int a=sw[idx].first,b=sw[idx].second;
        if(a!=pos&&b!=pos)continue;
        int np = (a==pos)?b:a;
        vector<int> nr; nr.reserve(rem.size()-1);
        for(int j=0;j<(int)rem.size();j++) if(j!=i) nr.push_back(rem[j]);
        solve(np,nr);
    }
}
int main(){
    cin>>N>>M;
    sw.resize(M);
    for(int i=0;i<M;i++){cin>>sw[i].first>>sw[i].second;}
    vector<int> rem(M); for(int i=0;i<M;i++)rem[i]=i;
    solve(1,rem);
    bool f=true;
    for(int x:res){if(!f)cout<<" ";cout<<x;f=false;}
    cout<<"\n";
}
