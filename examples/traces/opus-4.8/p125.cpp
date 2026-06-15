#include <bits/stdc++.h>
using namespace std;

int N;
int B[3][3];
int A[3][3];

bool checkCell(int i,int j){
    int cnt=0;
    int di[]={-1,1,0,0}, dj[]={0,0,-1,1};
    for(int k=0;k<4;k++){
        int ni=i+di[k], nj=j+dj[k];
        if(ni<0||ni>=N||nj<0||nj>=N) continue;
        if(A[ni][nj]>A[i][j]) cnt++;
    }
    return cnt==B[i][j];
}

bool solve(int idx){
    if(idx==N*N){
        for(int j=0;j<N;j++) if(!checkCell(N-1,j)) return false;
        return true;
    }
    int i=idx/N, j=idx%N;
    for(int v=0;v<=9;v++){
        A[i][j]=v;
        if(i>0){
            if(!checkCell(i-1,j)) continue;
        }
        if(solve(idx+1)) return true;
    }
    return false;
}

int main(){
    scanf("%d",&N);
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            scanf("%d",&B[i][j]);
    if(solve(0)){
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                printf("%d",A[i][j]);
                if(j<N-1) printf(" ");
            }
            printf("\n");
        }
    } else {
        printf("NO SOLUTION\n");
    }
    return 0;
}
