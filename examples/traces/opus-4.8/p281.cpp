#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    scanf("%d", &n);
    vector<string> a(n), b(n);
    for(int i=0;i<n;i++){char buf[64]; scanf("%s", buf); a[i]=buf;}
    for(int i=0;i<n;i++){char buf[64]; scanf("%s", buf); b[i]=buf;}
    
    unordered_map<string,int> cnt;
    cnt.reserve(n*2+10);
    int both=0;
    vector<int> breaks;
    for(int i=0;i<n;i++){
        if(++cnt[a[i]]==2) both++;
        if(++cnt[b[i]]==2) both++;
        if(both==i+1) breaks.push_back(i+1);
    }
    vector<string> result;
    result.reserve(n);
    int prev=0;
    for(int bp : breaks){
        vector<string> seg;
        for(int j=prev;j<bp;j++) seg.push_back(a[j]);
        sort(seg.begin(), seg.end());
        for(auto&s:seg) result.push_back(s);
        prev=bp;
    }
    for(auto&s:result) printf("%s\n", s.c_str());
    return 0;
}
