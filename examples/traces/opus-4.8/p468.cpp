#include <bits/stdc++.h>
using namespace std;

int N;
int dx[8]={1,1,-1,-1,2,2,-2,-2};
int dy[8]={2,-2,2,-2,1,-1,1,-1};

vector<int> board; // N*N, 0 unvisited else order

inline bool inside(int x,int y){return x>=0&&x<N&&y>=0&&y<N;}

int countDeg(int x,int y){
    int c=0;
    for(int k=0;k<8;k++){
        int nx=x+dx[k],ny=y+dy[k];
        if(inside(nx,ny)&&board[nx*N+ny]==0)c++;
    }
    return c;
}

bool attempt(int sx,int sy,int tieMode){
    fill(board.begin(),board.end(),0);
    int cx=sx,cy=sy;
    board[cx*N+cy]=1;
    double cc=(N-1)/2.0;
    for(int step=2;step<=N*N;step++){
        int best=-1;
        int bestDeg=100;
        double bestVal=-1;
        for(int k=0;k<8;k++){
            int nx=cx+dx[k],ny=cy+dy[k];
            if(!inside(nx,ny)||board[nx*N+ny]!=0)continue;
            int d=countDeg(nx,ny);
            double val;
            // tie-break value: distance from center
            double ddx=nx-cc, ddy=ny-cc;
            double dist=ddx*ddx+ddy*ddy;
            if(tieMode==0)val=dist;        // prefer far
            else if(tieMode==1)val=-dist;  // prefer near
            else val=nx*1000.0+ny;         // arbitrary
            if(d<bestDeg||(d==bestDeg&&val>bestVal)){
                bestDeg=d;bestVal=val;best=k;
            }
        }
        if(best<0)return false;
        cx+=dx[best];cy+=dy[best];
        board[cx*N+cy]=step;
    }
    return true;
}

int main(){
    if(!(cin>>N))return 0;
    if(N==1){
        cout<<"There is solution:\n";
        cout<<" 1\n";
        return 0;
    }
    if(N<5){
        cout<<"No solution.\n";
        return 0;
    }
    board.assign(N*N,0);
    // try several starts and tie-break modes
    vector<pair<int,int>> starts={{0,0},{0,N-1},{N-1,0},{N-1,N-1},{0,1},{1,0},{N/2,N/2}};
    bool found=false;
    for(int tm=0;tm<3&&!found;tm++){
        for(auto&s:starts){
            if(attempt(s.first,s.second,tm)){found=true;break;}
        }
    }
    if(!found){
        // brute try more starts with mode 0
        for(int i=0;i<N&&!found;i++)for(int j=0;j<N&&!found;j++){
            if(attempt(i,j,0)){found=true;}
        }
    }
    if(!found){
        cout<<"No solution.\n";
        return 0;
    }
    cout<<"There is solution:\n";
    // build output
    {
        string out;
        out.reserve(N*N*7);
        char buf[16];
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                int v=board[i*N+j];
                sprintf(buf,"%d",v);
                out.push_back(' ');
                out+=buf;
            }
            out.push_back('\n');
        }
        cout<<out;
    }
    return 0;
}
