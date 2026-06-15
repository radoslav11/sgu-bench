#include <bits/stdc++.h>
using namespace std;
int n;
int y[2005];
bool vis[2005][2005];
int main(){
    scanf("%d",&n);
    for(int i=0;i<n;i++) scanf("%d",&y[i]);
    int m=n-1;
    auto lo=[&](int e){return min(y[e],y[e+1]);};
    auto hi=[&](int e){return max(y[e],y[e+1]);};
    auto overlap=[&](int a,int b){
        return max(lo(a),lo(b))<=min(hi(a),hi(b));
    };
    queue<pair<int,int>>q;
    if(overlap(0,m-1)){vis[0][m-1]=1;q.push({0,m-1});}
    bool ans=false;
    while(!q.empty()){
        auto[i,j]=q.front();q.pop();
        if(i==j){ans=true;break;}
        int di[]={-1,1,0,0};
        int dj[]={0,0,-1,1};
        for(int d=0;d<4;d++){
            int ni=i+di[d],nj=j+dj[d];
            if(ni<0||ni>=m||nj<0||nj>=m)continue;
            if(vis[ni][nj])continue;
            if(overlap(ni,nj)){vis[ni][nj]=1;q.push({ni,nj});}
        }
    }
    printf(ans?"YES\n":"NO\n");
    return 0;
}
