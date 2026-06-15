#include <bits/stdc++.h>
using namespace std;

int main() {
    int T, N;
    scanf("%d %d", &T, &N);
    
    vector<int> qty(N);
    for (int i = 0; i < N; i++) scanf("%d", &qty[i]);
    
    vector<vector<double>> price(T+1, vector<double>(N));
    for (int t = 0; t <= T; t++)
        for (int i = 0; i < N; i++)
            scanf("%lf", &price[t][i]);
    
    vector<vector<double>> ret(T, vector<double>(N));
    for (int t = 1; t <= T; t++)
        for (int i = 0; i < N; i++)
            ret[t-1][i] = (price[t-1][i] - price[t][i]) / price[t][i];
    
    vector<double> mi(N, 0.0);
    for (int i = 0; i < N; i++) {
        for (int t = 0; t < T; t++) mi[i] += ret[t][i];
        mi[i] /= T;
    }
    
    vector<vector<double>> S(N, vector<double>(N, 0.0));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            for (int t = 0; t < T; t++)
                S[i][j] += (ret[t][i] - mi[i]) * (ret[t][j] - mi[j]);
            S[i][j] /= T;
        }
    
    double VP = 0.0;
    vector<double> Vi(N);
    for (int i = 0; i < N; i++) {
        Vi[i] = qty[i] * price[0][i];
        VP += Vi[i];
    }
    
    vector<double> w(N);
    for (int i = 0; i < N; i++) w[i] = Vi[i] / VP;
    
    double mP = 0.0;
    for (int i = 0; i < N; i++) mP += w[i] * mi[i];
    
    double var = 0.0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            var += w[i] * S[i][j] * w[j];
    double sP = sqrt(var);
    
    double VaR = -VP * (mP - 1.644854 * sP);
    
    printf("%.2f\n", VaR);
    
    return 0;
}
