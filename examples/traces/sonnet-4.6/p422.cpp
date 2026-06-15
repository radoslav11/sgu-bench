#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, t;
    cin >> n >> t;
    vector<double> a(n+1);
    for(int i = 1; i <= n; i++) cin >> a[i];
    
    vector<double> dp(n+1, 0.0);
    // dp[n] = 0
    // dp[i] = min over k=1..n-i of { k + t + sum_{j=1}^{k} P(j-1)*a[i+j]*(k-j+1+dp[i+j-1]) + P(k)*dp[i+k] }
    
    for(int i = n-1; i >= 0; i--){
        double best = 1e18;
        double Pprod = 1.0; // P(j-1) as j goes 1..k, so initially P(0)=1
        double sumErr = 0.0; // sum_{j=1}^{k} P(j-1)*a[i+j]*(dp[i+j-1])
        double sumErrCoeff = 0.0; // sum_{j=1}^{k} P(j-1)*a[i+j]*(-j)  [for the (k-j+1) part]
        double sumErrProb = 0.0; // sum_{j=1}^{k} P(j-1)*a[i+j]  [coefficient of k+1]
        
        for(int k = 1; k <= n-i; k++){
            int j = k;
            double pj1 = Pprod; // P(j-1) = P(k-1)
            double contrib = pj1 * a[i+j];
            sumErr += contrib * dp[i+j-1];
            sumErrCoeff += contrib * (-j);
            sumErrProb += contrib;
            Pprod *= (1.0 - a[i+j]);
            
            double val = (double)k + t + (k+1)*sumErrProb + sumErrCoeff + sumErr + Pprod * dp[i+k];
            if(val < best) best = val;
        }
        dp[i] = best;
    }
    
    cout << fixed << setprecision(15) << dp[0] << endl;
    return 0;
}
