#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M; cin>>N>>M;
    vector<string> board(N);
    int qr,qc;
    for(int i=0;i<N;i++){cin>>board[i];for(int j=0;j<N;j++)if(board[i][j]=='Q'){qr=i;qc=j;}}
    board[qr][qc]='.';
    vector<vector<bool>> cur(N,vector<bool>(N,false));
    cur[qr][qc]=true;
    int dx[]={-1,-1,-1,0,0,1,1,1};
    int dy[]={-1,0,1,-1,1,-1,0,1};
    for(int step=0;step<M;step++){
        vector<vector<bool>> nxt(N,vector<bool>(N,false));
        for(int r=0;r<N;r++)for(int c=0;c<N;c++){
            if(!cur[r][c])continue;
            for(int d=0;d<8;d++){
                int nr=r+dx[d],nc=c+dy[d];
                while(nr>=0&&nr<N&&nc>=0&&nc<N){
                    if(board[nr][nc]=='W')break;
                    nxt[nr][nc]=true;
                    if(board[nr][nc]=='B')break;
                    nr+=dx[d];nc+=dy[d];
                }
            }
        }
        cur=nxt;
    }
    int ans=0;
    for(int r=0;r<N;r++)for(int c=0;c<N;c++)ans+=cur[r][c];
    cout<<ans<<endl;
}
