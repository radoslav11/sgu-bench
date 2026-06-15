#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M,K;
    cin>>N>>M>>K;
    vector<vector<int>> g(N,vector<int>(M));
    vector<vector<int>> pr(N,vector<int>(M+1,0)), pc(M,vector<int>(N+1,0));
    for(int i=0;i<N;i++) for(int j=0;j<M;j++){cin>>g[i][j];}
    for(int i=0;i<N;i++) for(int j=0;j<M;j++) pr[i][j+1]=pr[i][j]+g[i][j];
    for(int j=0;j<M;j++) for(int i=0;i<N;i++) pc[j][i+1]=pc[j][i]+g[i][j];
    int best=-1,br=-1,bc=-1,bc1=-1,bc2=-1,br1=-1,br2=-1;
    for(int r=0;r<N;r++) for(int c=0;c<M;c++)
      for(int c1=0;c1<=c;c1++) for(int c2=c;c2<M;c2++)
        for(int r1=0;r1<=r;r1++) for(int r2=r;r2<N;r2++){
          int cells=(c2-c1+1)+(r2-r1+1)-1;
          if(cells!=K) continue;
          int oil=(pr[r][c2+1]-pr[r][c1])+(pc[c][r2+1]-pc[c][r1])-g[r][c];
          if(oil>best){best=oil;br=r;bc=c;bc1=c1;bc2=c2;br1=r1;br2=r2;}
        }
    cout<<"Oil : "<<best<<"\n";
    set<pair<int,int>> used;
    for(int j=bc1;j<=bc2;j++) used.insert({br,j});
    for(int i=br1;i<=br2;i++) used.insert({i,bc});
    for(auto&p:used) cout<<p.first+1<<" "<<p.second+1<<"\n";
}
