#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    scanf("%d",&n);
    unordered_map<string,int> cnt;
    cnt.reserve(1<<20);
    cnt.max_load_factor(0.7);
    
    for(int i=0;i<n;i++){
        int k;
        scanf("%d",&k);
        vector<string> props(k);
        for(int j=0;j<k;j++){
            char buf[64];
            scanf("%s",buf);
            props[j]=buf;
        }
        // generate all non-empty subsets
        for(int mask=1;mask<(1<<k);mask++){
            vector<string> sub;
            for(int j=0;j<k;j++) if(mask&(1<<j)) sub.push_back(props[j]);
            sort(sub.begin(),sub.end());
            string key;
            for(auto&s:sub){ key+=s; key+='\n'; }
            cnt[key]++;
        }
    }
    
    int m;
    scanf("%d",&m);
    string out;
    for(int i=0;i<m;i++){
        int l;
        scanf("%d",&l);
        vector<string> props(l);
        for(int j=0;j<l;j++){
            char buf[64];
            scanf("%s",buf);
            props[j]=buf;
        }
        sort(props.begin(),props.end());
        string key;
        for(auto&s:props){ key+=s; key+='\n'; }
        auto it=cnt.find(key);
        out += to_string(it==cnt.end()?0:it->second);
        out += '\n';
    }
    printf("%s",out.c_str());
    return 0;
}
