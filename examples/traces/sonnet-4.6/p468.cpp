#include <bits/stdc++.h>
using namespace std;
int N;
int board[255][255];
int dx[]={1,2,2,1,-1,-2,-2,-1};
int dy[]={2,1,-1,-2,-2,-1,1,2};
int degree(int x,int y){int c=0;for(int i=0;i<8;i++){int nx=x+dx[i],ny=y+dy[i];if(nx>=0&&nx<N&&ny>=0&&ny<N&&board[nx][ny]==0)c++;}return c;}
bool solve(int sx,int sy){memset(board,0,sizeof(board));int x=sx,y=sy;board[x][y]=1;for(int step=2;step<=N*N;step++){int best=-1,bx=-1,by=-1;for(int i=0;i<8;i++){int nx=x+dx[i],ny=y+dy[i];if(nx>=0&&nx<N&&ny>=0&&ny<N&&board[nx][ny]==0){int d=degree(nx,ny);if(best==-1||d<best){best=d;bx=nx;by=ny;}}}if(bx==-1)return false;board[bx][by]=step;x=bx;y=by;}return true;}
int main(){cin>>N;if(N==1){cout<<"There is solution:\n1\n";return 0;}if(N<=4){cout<<"No solution.\n";return 0;}bool found=false;for(int si=0;si<N&&!found;si++)for(int sj=0;sj<N&&!found;sj++)if(solve(si,sj))found=true;if(!found)cout<<"No solution.\n";else{cout<<"There is solution:\n";for(int i=0;i<N;i++){for(int j=0;j<N;j++){if(j)cout<<" ";cout<<board[i][j];}cout<<"\n";}}return 0;}
