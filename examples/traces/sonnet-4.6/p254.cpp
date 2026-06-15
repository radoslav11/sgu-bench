#include <bits/stdc++.h>
using namespace std;
int bit[2000002],N;
void upd(int i,int v){for(;i<=N;i+=i&-i)bit[i]+=v;}
int qry(int i){int s=0;for(;i>0;i-=i&-i)s+=bit[i];return s;}
int kth(int k){int p=0;for(int pw=1<<21;pw;pw>>=1)if(p+pw<=N&&bit[p+pw]<k){p+=pw;k-=bit[p];}return p+1;}
int main(){
    int Q;string t;
    while(cin>>t)if(t.size()&&isdigit(t[0])){N=stoi(t);cin>>Q;break;}
    for(int i=1;i<=N;i++)upd(i,1);
    int cur=1,dir=1;
    for(int iter=0;iter<N;iter++){
        int sz=N-iter;
        int rank=qry(cur-1)+1;
        int nrank=((rank-1+(long long)dir*(Q-1))%sz+sz)%sz+1;
        int val=kth(nrank);
        upd(val,-1);
        int sz2=sz-1;
        if(sz2==0){printf("%d\n",val);return 0;}
        int nxtrank=nrank%sz2+1;
        int nxt=kth(nxtrank);
        dir=(nxt%2==1)?1:-1;
        cur=nxt;
    }
}
