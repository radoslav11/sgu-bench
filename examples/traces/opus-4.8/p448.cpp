#include <bits/stdc++.h>
using namespace std;
int N,M,R[17][17];long long f[17][17],C[20][20];
int h(int k){int r=0;while((1<<r)<k)r++;return r;}
int main(){
    cin>>N>>M;M--;
    for(int i=0;i<N;i++)for(int j=0;j<N;j++)cin>>R[i][j];
    for(int i=0;i<20;i++){C[i][0]=1;for(int j=1;j<=i;j++)C[i][j]=C[i-1][j-1]+C[i-1][j];}
    for(int i=0;i<N;i++)f[1][i]=1;
    for(int c=2;c<=N;c++){
        int hc=h(c);
        for(int a=1;a<c;a++){
            int b=c-a;
            if(h(a)>hc-1||h(b)>hc-1)continue;
            if(max(h(a),h(b))!=hc-1)continue;
            for(int i=0;i<N;i++){
                if(!f[a][i])continue;
                for(int j=0;j<N;j++){
                    if(i==j||!f[b][j])continue;
                    if(R[i][j]==1)f[c][i]+=f[a][i]*f[b][j]*C[c-2][a-1];
                }
            }
        }
    }
    cout<<f[N][M]<<endl;
}
