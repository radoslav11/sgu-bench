#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M; scanf("%d%d",&N,&M);
    int maxn=N+200010;
    vector<unordered_set<int>> out(maxn),in(maxn);
    for(int i=0;i<M;i++){int a,b;scanf("%d%d",&a,&b);out[a].insert(b);in[b].insert(a);}
    int Q; scanf("%d",&Q);
    int nxt=N+1;
    while(Q--){
        int t,a,b; scanf("%d%d%d",&t,&a,&b);
        if(t==1){puts(out[a].count(b)?"YES":"NO");}
        else{
            int c=nxt++;
            for(int x:in[a]){out[x].erase(a);out[x].insert(c);}
            for(int x:in[b]){out[x].erase(b);out[x].insert(c);}
            for(int x:out[a]){in[x].erase(a);in[x].insert(c);}
            for(int x:out[b]){in[x].erase(b);in[x].insert(c);}
            out[c]=move(out[a]);for(int x:out[b])out[c].insert(x);out[b].clear();
            in[c]=move(in[a]);for(int x:in[b])in[c].insert(x);in[b].clear();
        }
    }
}
