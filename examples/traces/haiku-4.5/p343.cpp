#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T, N;
    cin >> T >> N;
    
    vector<int> quantity(N);
    for(int i = 0; i < N; i++){
        cin >> quantity[i];
    }
    
    vector<vector<double>> prices(T+1, vector<double>(N));
    for(int t = 0; t <= T; t++){
        for(int i = 0; i < N; i++){
            cin >> prices[t][i];
        }
    }
    
    // Calculate portfolio value VP
    double VP = 0;
    for(int i = 0; i < N; i++){
        VP += quantity[i] * prices[0][i];
    }
    
    // Calculate weights wi
    vector<double> w(N);
    for(int i = 0; i < N; i++){
        w[i] = (quantity[i] * prices[0][i]) / VP;
    }
    
    // Calculate returns for each asset
    vector<vector<double>> returns(T, vector<double>(N));
    for(int t = 0; t < T; t++){
        for(int i = 0; i < N; i++){
            returns[t][i] = (prices[t][i] - prices[t+1][i]) / prices[t+1][i];
        }
    }
    
    // Calculate expected returns mi
    vector<double> m(N, 0);
    for(int i = 0; i < N; i++){
        for(int t = 0; t < T; t++){
            m[i] += returns[t][i];
        }
        m[i] /= T;
    }
    
    // Calculate mP
    double mP = 0;
    for(int i = 0; i < N; i++){
        mP += w[i] * m[i];
    }
    
    // Build covariance matrix S
    vector<vector<double>> S(N, vector<double>(N, 0));
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            double cov = 0;
            for(int t = 0; t < T; t++){
                cov += (returns[t][i] - m[i]) * (returns[t][j] - m[j]);
            }
            S[i][j] = cov / T;
        }
    }
    
    // Calculate sP = sqrt(W^T * S * W)
    double sP_squared = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            sP_squared += w[i] * S[i][j] * w[j];
        }
    }
    double sP = sqrt(sP_squared);
    
    // Calculate VaR
    double VaR = -VP * (mP - 1.644854 * sP);
    
    cout << fixed << setprecision(2) << VaR << endl;
    
    return 0;
}
