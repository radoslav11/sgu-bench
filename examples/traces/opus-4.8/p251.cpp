#include <bits/stdc++.h>
using namespace std;
int N;
int R[12];
int cross_[12][12];
int crossFinal[12][12];
bool found=false;

void dfs(int t){
    if(found) return;
    if(t==N){
        found=true;
        for(int i=0;i<N;i++)for(int j=0;j<N;j++) crossFinal[i][j]=cross_[i][j];
        return;
    }
    int cnt=t-1;
    for(int mask=0; mask<(1<<cnt); mask++){
        for(int s=1;s<t;s++){
            int bit=(mask>>(s-1))&1;
            int val=2*(R[0]+R[s]+R[t]);
            if(bit) val=-val;
            cross_[s][t]=val; cross_[t][s]=-val;
        }
        bool ok=true;
        for(int a=1;a<t&&ok;a++)for(int b=a+1;b<t&&ok;b++){
            int sum=cross_[a][b]+cross_[b][t]+cross_[t][a];
            if(abs(sum)!=2*(R[a]+R[b]+R[t])) ok=false;
        }
        if(!ok) continue;
        for(int p=1;p<t&&ok;p++)for(int q=p+1;q<t&&ok;q++)for(int r=q+1;r<t&&ok;r++){
            long long pf=(long long)cross_[p][q]*cross_[r][t]
                        -(long long)cross_[p][r]*cross_[q][t]
                        +(long long)cross_[p][t]*cross_[q][r];
            if(pf!=0) ok=false;
        }
        if(!ok) continue;
        dfs(t+1);
        if(found) return;
    }
}

int main(){
    if(!(cin>>N)) return 0;
    for(int i=0;i<N;i++) cin>>R[i];
    for(int i=0;i<12;i++)for(int j=0;j<12;j++) cross_[i][j]=0;
    dfs(1);
    if(!found){
        cout<<"NO\n";
        return 0;
    }
    cout<<"YES\n";
    // reconstruct: point 0 at origin
    // use a=1,b=2
    double Mab=(double)crossFinal[1][2];
    vector<double> X(N), Y(N);
    X[0]=0.0; Y[0]=0.0;
    for(int k=1;k<N;k++){
        X[k]=-(double)crossFinal[2][k]/Mab;
        Y[k]=(double)crossFinal[1][k];
    }
    cout<<fixed<<setprecision(4);
    for(int i=0;i<N;i++){
        cout<<X[i]<<" "<<Y[i]<<"\n";
    }
    return 0;
}
