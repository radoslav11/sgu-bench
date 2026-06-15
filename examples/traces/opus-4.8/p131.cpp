#include <bits/stdc++.h>
using namespace std;
int M,N;
void rec(int row,int cur,int nxt,long long val,long long* ndp){
    if(row==M){ ndp[nxt]+=val; return;}
    if(cur&(1<<row)){ rec(row+1,cur,nxt,val,ndp); return;}
    if(row+1<M && !(cur&(1<<(row+1))))
        rec(row+2,cur,nxt,val,ndp);
    if(!(nxt&(1<<row)))
        rec(row+1,cur,nxt|(1<<row),val,ndp);
    if(row+1<M && !(cur&(1<<(row+1)))){
        if(!(nxt&(1<<(row+1))))
            rec(row+2,cur,nxt|(1<<(row+1)),val,ndp);
        if(!(nxt&(1<<row)))
            rec(row+2,cur,nxt|(1<<row),val,ndp);
    }
    if(row+1<M){
        if(!(nxt&(1<<row)) && !(nxt&(1<<(row+1))))
            rec(row+1,cur,nxt|(1<<row)|(1<<(row+1)),val,ndp);
    }
}
int main(){
    cin>>M>>N;
    static long long cur[1<<9], nxt[1<<9];
    for(int i=0;i<(1<<M);i++) cur[i]=0;
    cur[0]=1;
    for(int c=0;c<N;c++){
        for(int i=0;i<(1<<M);i++) nxt[i]=0;
        for(int m=0;m<(1<<M);m++) if(cur[m]) rec(0,m,0,cur[m],nxt);
        for(int i=0;i<(1<<M);i++) cur[i]=nxt[i];
    }
    cout<<cur[0]<<endl;
}
