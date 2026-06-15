#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T;
    
    while(T--) {
        int n1, m1, n2, m2;
        cin >> n1 >> m1 >> n2 >> m2;
        
        double numerator = 0.0;
        double denominator = 0.0;
        int steps = 5000;
        double dq = 1.0 / steps;
        double dp = 1.0 / steps;
        
        for(int iq = 0; iq < steps; iq++) {
            double q = (iq + 0.5) * dq;
            double q_factor = pow(q, m2) * pow(1-q, n2-m2);
            
            for(int ip = 0; ip < steps; ip++) {
                double p = (ip + 0.5) * dp;
                double p_factor = pow(p, m1) * pow(1-p, n1-m1);
                double joint = p_factor * q_factor;
                
                denominator += joint * dp * dq;
                if(p < q) {
                    numerator += joint * dp * dq;
                }
            }
        }
        
        double prob = numerator / denominator;
        cout << fixed << setprecision(10) << prob << "\n";
    }
    
    return 0;
}
