#include <bits/stdc++.h>
using namespace std;
int main(){
    map<pair<int,int>,int> mp;
    char c; int L,R;
    string out;
    out.reserve(1<<20);
    while(scanf(" %c %d %d",&c,&L,&R)==3){
        if(c=='+'){
            long long cnt=0;
            for(auto&kv:mp){ if(L<=kv.first.first && kv.first.second<=R) cnt+=kv.second; }
            out+=to_string(cnt); out+='\n';
            mp[{L,R}]++;
        } else {
            auto it=mp.find({L,R});
            if(it!=mp.end()){ if(--it->second==0) mp.erase(it); }
        }
    }
    fputs(out.c_str(),stdout);
    return 0;
}
