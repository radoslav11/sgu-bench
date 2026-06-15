#include <bits/stdc++.h>
using namespace std;
int N,M;
vector<string> g;
int dx[8]={-1,-1,-1,0,0,1,1,1};
int dy[8]={-1,0,1,-1,1,-1,0,1};
int main(){
    cin>>N>>M;
    g.resize(N);
    for(auto&s:g)cin>>s;
    int qx=0,qy=0;
    for(int i=0;i<N;i++)for(int j=0;j<N;j++)if(g[i][j]=='Q'){qx=i;qy=j;}
    vector<char> cur(N*N,0), nxt(N*N,0), prev2(N*N,0);
    cur[qx*N+qy]=1;
    for(int step=0;step<M;step++){
        fill(nxt.begin(),nxt.end(),0);
        for(int i=0;i<N;i++)for(int j=0;j<N;j++){
            if(!cur[i*N+j])continue;
            for(int d=0;d<8;d++){
                int x=i+dx[d],y=j+dy[d];
                while(x>=0&&x<N&&y>=0&&y<N){
                    if(g[x][y]=='W')break;
                    if(g[x][y]=='B'){nxt[x*N+y]=1;break;}
                    nxt[x*N+y]=1;
                    x+=dx[d];y+=dy[d];
                }
            }
        }
        if(step>=1 && nxt==prev2){
            cur=nxt; break;
        }
        prev2=cur;
        swap(cur,nxt);
    }
    int cnt=0;
    for(int i=0;i<N*N;i++)if(cur[i])cnt++;
    cout<<cnt<<endl;
}
