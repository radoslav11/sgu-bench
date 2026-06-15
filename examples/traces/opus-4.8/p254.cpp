#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,Q;
    scanf("%d %d",&N,&Q);
    vector<int> nxt(N+1), prv(N+1);
    for(int i=1;i<=N;i++){ nxt[i]= (i==N)?1:i+1; prv[i]=(i==1)?N:i-1; }
    int cur=1;
    int dir=1;
    int remaining=N;
    int last=0;
    while(remaining>0){
        int p=cur;
        int steps=(Q-1)%remaining;
        for(int s=0;s<steps;s++){
            p = dir? nxt[p] : prv[p];
        }
        last=p;
        int pn=nxt[p], pp=prv[p];
        nxt[pp]=pn; prv[pn]=pp;
        remaining--;
        if(remaining==0) break;
        cur = pn;
        if(cur%2==1) dir=1; else dir=0;
    }
    printf("%d\n",last);
    return 0;
}
