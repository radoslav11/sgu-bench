#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
const ull INF = ~0ULL;
int N, M;
ull f[65][66];
int bestk[65][66];
ull solve(int n, int m){
    if(n==0) return 0;
    if(n==1) return 1;
    if(f[n][m]!=INF) return f[n][m];
    ull res=INF; int bk=1;
    for(int k=1;k<n;k++){
        ull a=solve(k,m);
        ull b=solve(n-k,m-1);
        ull cand = 2*a + b;
        if(cand<res){res=cand;bk=k;}
    }
    f[n][m]=res; bestk[n][m]=bk;
    return res;
}
vector<int> peg[66];
void output_move(int disk, int from, int to){
    peg[from].pop_back();
    if(peg[to].empty()) printf("move %d from %d to %d\n", disk, from, to);
    else printf("move %d from %d to %d atop %d\n", disk, from, to, peg[to].back());
    peg[to].push_back(disk);
}
void move(int n, int from, int to, vector<int> avail){
    if(n==0) return;
    if(n==1){ output_move(peg[from].back(), from, to); return; }
    int m = avail.size();
    int inter=-1;
    for(int p: avail) if(p!=from && p!=to){inter=p;break;}
    int k = bestk[n][m];
    move(k, from, inter, avail);
    vector<int> avail2;
    for(int p: avail) if(p!=inter) avail2.push_back(p);
    move(n-k, from, to, avail2);
    move(k, inter, to, avail);
}
int main(){
    scanf("%d %d",&N,&M);
    for(int i=0;i<=64;i++)for(int j=0;j<=65;j++)f[i][j]=INF;
    ull ans = solve(N, M);
    printf("%llu\n", ans);
    for(int d=N; d>=1; d--) peg[1].push_back(d);
    vector<int> avail;
    for(int p=1;p<=M;p++) avail.push_back(p);
    move(N, 1, M, avail);
    return 0;
}
