#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
int K; ull N;
int board[32], ini[32];
vector<vector<int>> grps;
vector<int> cg[32];
int gm[30];
bool isv(int c,int v){int b=1<<v;for(int g:cg[c])if(gm[g]&b)return 0;return 1;}
void pl(int c,int v){board[c]=v;int b=1<<v;for(int g:cg[c])gm[g]|=b;}
void up(int c,int v){board[c]=0;int b=1<<v;for(int g:cg[c])gm[g]^=b;}
ull CAP;
ull ct(int p){
    if(p==32)return 1;
    if(ini[p])return ct(p+1);
    ull t=0;
    for(int v=1;v<=K;v++){
        if(isv(p,v)){
            pl(p,v);t+=ct(p+1);up(p,v);
            if(t>=CAP)return CAP;
        }
    }
    return t;
}
bool found;
void solve(int p, ull need){
    if(found)return;
    if(p==32){found=1;return;}
    if(ini[p]){solve(p+1,need);return;}
    for(int v=1;v<=K;v++){
        if(isv(p,v)){
            pl(p,v);
            CAP=need+1;
            ull c=ct(p+1);
            if(c>=need){solve(p+1,need);if(found)return;}
            else need-=c;
            up(p,v);
        }
    }
}
int main(){
    ios::sync_with_stdio(0);cin.tie(0);
    cin>>K>>N;
    for(int i=1;i<=31;i++)cin>>ini[i];
    grps={{1,2},{3,4,5,6,7},{8,9,10,11,12,13},{14,15,16,17,18},{19,20,21,22,23,24},{25,26,27,28,29},{30,31},
          {3,8},{1,4,9,14,19},{2,5,10,15,20,25},{6,11,16,21,26},{7,12,17,22,27,30},{13,18,23,28,31},{24,29},
          {7,13},{2,6,12,18,24},{1,5,11,17,23,29},{4,10,16,22,28},{3,9,15,21,27,31},{8,14,20,26,30},{19,25},
          {1,2,4,5,6,10,11},{3,4,8,9,10,14,15},{6,7,11,12,13,17,18},{10,11,15,16,17,21,22},{14,15,19,20,21,25,26},{17,18,22,23,24,28,29},{21,22,26,27,28,30,31}};
    for(int i=0;i<(int)grps.size();i++)for(int c:grps[i])cg[c].push_back(i);
    memset(gm,0,sizeof(gm));
    for(int i=1;i<=31;i++)if(ini[i]){int b=1<<ini[i];for(int g:cg[i])gm[g]|=b;}
    found=0;solve(1,N);
    if(found){cout<<"Found\n";for(int i=1;i<=31;i++)cout<<board[i]<<(i<31?" ":"\n");}
    else cout<<"No way\n";
}
