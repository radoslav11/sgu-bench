#include <bits/stdc++.h>
using namespace std;
int n,m;
char board[15][16];
vector<pair<int,int>> pieces;
int np;
bool inB(int r,int c){return r>=0&&r<n&&c>=0&&c<m;}
void computeAttacked(int kr,int kc,int mask,bool att[][15]){
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)att[i][j]=false;
    for(int i=0;i<np;i++){
        if(!(mask&(1<<i)))continue;
        int pr=pieces[i].first,pc=pieces[i].second;
        char t=board[pr][pc];
        if(t=='K'){
            int dr[]={-2,-2,-1,-1,1,1,2,2},dc[]={-1,1,-2,2,-2,2,-1,1};
            for(int d=0;d<8;d++){int nr=pr+dr[d],nc=pc+dc[d];if(inB(nr,nc))att[nr][nc]=true;}
        } else {
            int dirs[4][2]={};
            if(t=='B'){dirs[0][0]=1;dirs[0][1]=1;dirs[1][0]=1;dirs[1][1]=-1;dirs[2][0]=-1;dirs[2][1]=1;dirs[3][0]=-1;dirs[3][1]=-1;}
            else{dirs[0][0]=1;dirs[0][1]=0;dirs[1][0]=-1;dirs[1][1]=0;dirs[2][0]=0;dirs[2][1]=1;dirs[3][0]=0;dirs[3][1]=-1;}
            for(int d=0;d<4;d++){
                int r=pr+dirs[d][0],c=pc+dirs[d][1];
                while(inB(r,c)){
                    att[r][c]=true;
                    bool bl=(r==kr&&c==kc);
                    if(!bl)for(int j=0;j<np;j++)if((mask&(1<<j))&&j!=i&&pieces[j].first==r&&pieces[j].second==c){bl=true;break;}
                    if(bl)break;
                    r+=dirs[d][0];c+=dirs[d][1];
                }
            }
        }
    }
}
int dist[15][15][1<<14];
int main(){
    ios::sync_with_stdio(false);cin.tie(0);
    cin>>n>>m;
    int skr=-1,skc=-1;
    for(int i=0;i<n;i++){
        cin>>board[i];
        for(int j=0;j<m;j++){
            if(board[i][j]=='*'){skr=i;skc=j;}
            else if(board[i][j]!='.')pieces.push_back({i,j});
        }
    }
    np=pieces.size();
    if(np==0){cout<<0;return 0;}
    int full=(1<<np)-1;
    memset(dist,-1,sizeof(dist));
    dist[skr][skc][full]=0;
    queue<tuple<int,int,int>>q;
    q.push({skr,skc,full});
    bool att[15][15],att2[15][15];
    int ddr[]={-1,-1,-1,0,0,1,1,1},ddc[]={-1,0,1,-1,1,-1,0,1};
    while(!q.empty()){
        auto[r,c,mask]=q.front();q.pop();
        int d=dist[r][c][mask];
        if(mask==0){cout<<d;return 0;}
        computeAttacked(r,c,mask,att);
        for(int i=0;i<8;i++){
            int nr=r+ddr[i],nc=c+ddc[i];
            if(!inB(nr,nc))continue;
            int nmask=mask;
            int pidx=-1;
            for(int j=0;j<np;j++)if((mask&(1<<j))&&pieces[j].first==nr&&pieces[j].second==nc){pidx=j;break;}
            if(pidx>=0){
                nmask=mask^(1<<pidx);
                computeAttacked(nr,nc,nmask,att2);
                if(att2[nr][nc])continue;
            } else {
                if(att[nr][nc])continue;
            }
            if(dist[nr][nc][nmask]==-1){dist[nr][nc][nmask]=d+1;q.push({nr,nc,nmask});}
        }
    }
    cout<<-1;
}
