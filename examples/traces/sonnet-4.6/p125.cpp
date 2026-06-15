#include <bits/stdc++.h>
using namespace std;
int N, B[3][3], A[3][3];
int dx[]={0,0,1,-1}, dy[]={1,-1,0,0};

bool partialCheck(int i, int j, int pos){
    int cnt=0, unknown=0;
    for(int d=0;d<4;d++){
        int ni=i+dx[d],nj=j+dy[d];
        if(ni<0||ni>=N||nj<0||nj>=N) continue;
        int npos=ni*N+nj;
        if(npos<pos){if(A[ni][nj]>A[i][j])cnt++;}
        else unknown++;
    }
    if(cnt>B[i][j]) return false;
    if(cnt+unknown<B[i][j]) return false;
    return true;
}

bool solve(int pos){
    if(pos==N*N) return true;
    int i=pos/N,j=pos%N;
    for(int v=0;v<=9;v++){
        A[i][j]=v;
        bool ok=true;
        if(!partialCheck(i,j,pos+1)) ok=false;
        if(ok){
            for(int d=0;d<4&&ok;d++){
                int ni=i+dx[d],nj=j+dy[d];
                if(ni>=0&&ni<N&&nj>=0&&nj<N&&ni*N+nj<pos)
                    if(!partialCheck(ni,nj,pos+1)) ok=false;
            }
        }
        if(ok&&solve(pos+1)) return true;
    }
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>N;
    for(int i=0;i<N;i++)for(int j=0;j<N;j++)cin>>B[i][j];
    if(solve(0)){
        for(int i=0;i<N;i++){for(int j=0;j<N;j++){if(j)cout<<' ';cout<<A[i][j];}cout<<'\n';}
    } else cout<<"NO SOLUTION"<<endl;
}
