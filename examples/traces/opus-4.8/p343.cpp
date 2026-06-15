#include <bits/stdc++.h>
using namespace std;

int main(){
    int T, N;
    scanf("%d %d", &T, &N);
    vector<double> qty(N);
    for(int i=0;i<N;i++) scanf("%lf",&qty[i]);
    
    vector<vector<double>> price(T+1, vector<double>(N));
    for(int t=0;t<=T;t++)
        for(int i=0;i<N;i++)
            scanf("%lf",&price[t][i]);
    
    vector<vector<double>> ret(T, vector<double>(N));
    for(int t=1;t<=T;t++)
        for(int i=0;i<N;i++)
            ret[t-1][i] = (price[t-1][i]-price[t][i])/price[t][i];
    
    vector<double> m(N,0.0);
    for(int i=0;i<N;i++){
        double s=0;
        for(int t=0;t<T;t++) s+=ret[t][i];
        m[i]=s/T;
    }
    
    vector<vector<double>> S(N, vector<double>(N,0.0));
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++){
            double s=0;
            for(int t=0;t<T;t++)
                s += (ret[t][i]-m[i])*(ret[t][j]-m[j]);
            S[i][j]=s/T;
        }
    
    vector<double> V(N);
    double VP=0;
    for(int i=0;i<N;i++){ V[i]=qty[i]*price[0][i]; VP+=V[i]; }
    vector<double> w(N);
    for(int i=0;i<N;i++) w[i]=V[i]/VP;
    
    double mP=0;
    for(int i=0;i<N;i++) mP += w[i]*m[i];
    
    double var=0;
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            var += w[i]*S[i][j]*w[j];
    double sP = sqrt(var);
    
    double VaR = -VP*(mP - 1.644854*sP);
    
    printf("%.2f\n", VaR);
    return 0;
}
