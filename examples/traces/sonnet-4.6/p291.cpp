#include <bits/stdc++.h>
using namespace std;
int main(){
    int q,c,n,t;
    scanf("%d%d%d%d",&q,&c,&n,&t);
    vector<int> owner(q*c,-1);
    vector<int> dist(q*c,INT_MAX);
    priority_queue<tuple<int,int,int>,vector<tuple<int,int,int>>,greater<>> pq;
    for(int i=0;i<n;i++){
        int x,y; scanf("%d%d",&x,&y); x--;y--;
        int cell=x*c+y;
        pq.push({0,i,cell});
    }
    int dx[]={0,0,1,-1},dy[]={1,-1,0,0};
    while(!pq.empty()){
        auto[d,cu,cell]=pq.top();pq.pop();
        if(dist[cell]!=INT_MAX)continue;
        if(d>t)break;
        dist[cell]=d;owner[cell]=cu;
        int x=cell/c,y=cell%c;
        for(int k=0;k<4;k++){int nx=x+dx[k],ny=y+dy[k];if(nx>=0&&nx<q&&ny>=0&&ny<c){int nc=nx*c+ny;if(dist[nc]==INT_MAX)pq.push({d+1,cu,nc});}}
    }
    vector<int> cnt(n,0);
    for(int i=0;i<q*c;i++)if(owner[i]>=0)cnt[owner[i]]++;
    for(int i=0;i<n;i++)printf("%d\n",cnt[i]);
}
