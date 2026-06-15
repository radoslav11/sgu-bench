#include <bits/stdc++.h>
using namespace std;
int n,S,T,adj[13],pop[13];
map<tuple<int,int,int>,int> memo;
int tp(int m){int s=0;for(int i=0;i<n;i++)if(m>>i&1)s+=pop[i];return s;}
int nb(int m){int r=0;for(int i=0;i<n;i++)if(m>>i&1)r|=adj[i];return r&~m;}
int solve(int A,int B,int turn){
auto k=make_tuple(A,B,turn);auto it=memo.find(k);if(it!=memo.end())return it->second;
int res;
if(turn==0){int av=nb(A)&~B,pA=tp(A);res=solve(A,B,1);for(int C=av;C;C=(C-1)&av)if(tp(C)<=pA)res=max(res,solve(A|C,B,1));}
else{int av=nb(B)&~A,pB=tp(B);int avA=nb(A)&~B,pA=tp(A);bool p1=false;for(int C=avA;C;C=(C-1)&avA)if(tp(C)<=pA){p1=true;break;}bool p2=false;for(int C=av;C;C=(C-1)&av)if(tp(C)<=pB){p2=true;break;}if(!p1&&!p2){res=tp(A)-tp(B);}else{res=solve(A,B,0);for(int C=av;C;C=(C-1)&av)if(tp(C)<=pB)res=min(res,solve(A,B|C,0));}}
return memo[k]=res;}
int main(){cin>>n>>S>>T;S--;T--;for(int i=0;i<n;i++){string s;cin>>s;for(int j=0;j<n;j++)if(s[j]=='1')adj[i]|=1<<j;}for(int i=0;i<n;i++)cin>>pop[i];cout<<solve(1<<S,1<<T,0)<<endl;}
