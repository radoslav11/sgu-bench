#include <bits/stdc++.h>
using namespace std;
int main(){
    int N; cin>>N;
    int M=(N%2==0)?N:N+1;
    int T=M-1;
    vector<vector<int>> s(M+1,vector<int>(M+1,0));
    for(int r=1;r<=T;r++){
        s[M][r]=r; s[r][M]=r;
        for(int i=1;i<=M/2-1;i++){
            int a=((r-1+i)%(M-1))+1;
            int b=((r-1-i%(M-1)+(long long)(M-1)*M)%(M-1))+1;
            s[a][b]=r; s[b][a]=r;
        }
    }
    cout<<T<<"\n";
    for(int i=1;i<=N;i++){for(int j=1;j<=N;j++){if(j>1)cout<<" ";cout<<s[i][j];}cout<<"\n";}
}
