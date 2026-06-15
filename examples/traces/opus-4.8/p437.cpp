#include <bits/stdc++.h>
using namespace std;
vector<int> conflict[32];
int K; long long N; int val[32]; int given[32];
const long long CAP=(long long)2e18;
bool ok(int idx,int v){ for(int c: conflict[idx]) if(c<idx && val[c]==v) return false; return true; }
long long countFrom(int idx){
    if(idx>31) return 1;
    long long total=0; int lo=1,hi=K;
    if(given[idx]) lo=hi=given[idx];
    for(int v=lo;v<=hi;v++) if(ok(idx,v)){ val[idx]=v; long long c=countFrom(idx+1); total+=c; if(total>CAP)total=CAP; }
    val[idx]=0; return total;
}
bool solve(int idx){
    if(idx>31) return true;
    int lo=1,hi=K; if(given[idx]) lo=hi=given[idx];
    for(int v=lo;v<=hi;v++) if(ok(idx,v)){ val[idx]=v; long long c=countFrom(idx+1);
        if(N<=c){ if(solve(idx+1)) return true; } else N-=c; }
    val[idx]=0; return false;
}
int main(){
    vector<vector<int>> lines = {
        {1,2},{3,4,5,6,7},{8,9,10,11,12,13},{14,15,16,17,18},{19,20,21,22,23,24},{25,26,27,28,29},{30,31},
        {3,8},{1,4,9,14,19},{2,5,10,15,20,25},{6,11,16,21,26},{7,12,17,22,27,30},{13,18,23,28,31},{24,29},
        {7,13},{2,6,12,18,24},{1,5,11,17,23,29},{4,10,16,22,28},{3,9,15,21,27,31},{8,14,20,26,30},{19,25},
        {1,2,4,5,6,10,11},{3,4,8,9,10,14,15},{6,7,11,12,13,17,18},{10,11,15,16,17,21,22},
        {14,15,19,20,21,25,26},{17,18,22,23,24,28,29},{21,22,26,27,28,30,31}
    };
    set<pair<int,int>> cf;
    for(auto&g:lines) for(int a:g)for(int b:g)if(a!=b) cf.insert({a,b});
    for(auto&p:cf) conflict[p.first].push_back(p.second);
    scanf("%d %lld",&K,&N);
    for(int i=1;i<=31;i++) scanf("%d",&given[i]);
    bool valid=true;
    for(int i=1;i<=31;i++) val[i]=given[i];
    for(int i=1;i<=31;i++) if(given[i]) for(int c:conflict[i]) if(c<i && val[c]==given[i]) valid=false;
    for(int i=1;i<=31;i++) val[i]=0;
    if(valid && solve(1)){
        printf("Found\n");
        for(int i=1;i<=31;i++) printf("%d%c", val[i], i<31?' ':'\n');
    } else printf("No way\n");
    return 0;
}
