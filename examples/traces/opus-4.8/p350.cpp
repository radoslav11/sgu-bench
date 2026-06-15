#include <bits/stdc++.h>
using namespace std;

int n;
vector<unsigned int> chosen;
map<unsigned int,int> cnt;

bool dfs(int needed, unsigned int lastVal){
    if(needed==0) return true;
    // gather distinct candidate values (descending)
    vector<unsigned int> cands;
    for(auto&pr:cnt) if(pr.second>0) cands.push_back(pr.first);
    sort(cands.rbegin(), cands.rend());
    for(unsigned int v:cands){
        if(v >= lastVal) continue; // enforce strictly decreasing order of real elements
        // required removals: v xor c for each chosen c
        map<unsigned int,int> need;
        for(unsigned int c:chosen) need[v^c]++;
        bool ok=true;
        for(auto&pr:need) if(cnt[pr.first]<pr.second){ok=false;break;}
        if(!ok) continue;
        for(auto&pr:need) cnt[pr.first]-=pr.second;
        chosen.push_back(v);
        if(dfs(needed-1, v)) return true;
        chosen.pop_back();
        for(auto&pr:need) cnt[pr.first]+=pr.second;
    }
    return false;
}

int main(){
    int M;
    if(!(cin>>M)) return 0;
    vector<unsigned int> B(M);
    for(int i=0;i<M;i++) cin>>B[i];
    // find n with n*(n-1)/2 == M
    n=0;
    for(int t=1;t<=1000;t++){
        if((long long)t*(t-1)/2==M){ n=t; break; }
    }
    // chosen starts with 0
    chosen.clear();
    chosen.push_back(0u);
    cnt.clear();
    for(unsigned int b:B) cnt[b]++;
    // need to add n-1 real elements
    if(n==1){
        // single element, B empty (M=0) shouldn't happen since M>=1
        cout<<0<<"\n";
        return 0;
    }
    dfs(n-1, UINT_MAX);
    // output chosen
    for(size_t i=0;i<chosen.size();i++){
        if(i) cout<<' ';
        cout<<chosen[i];
    }
    cout<<"\n";
    return 0;
}
