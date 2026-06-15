#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    double t;
    if (!(cin >> n >> t)) return 0;

    vector<double> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    vector<double> E(n + 1, 0.0);
    E[n] = 0.0;

    for (int j = n - 1; j >= 0; --j) {
        double min_A = 1e18;
        double P_fail_total = 0.0;
        double sum_fail = 0.0;
        
        for (int k = j + 1; k <= n; ++k) {
            double P_succ = 1.0 - P_fail_total;
            double p_fail_f = P_succ * a[k];
            
            double E_prev = (k - 1 == j) ? 0.0 : E[k - 1];
            sum_fail = sum_fail + P_fail_total + p_fail_f * (1.0 + E_prev);
            P_fail_total += p_fail_f;
            
            double current_A = (k - j) + t + sum_fail + (1.0 - P_fail_total) * E[k];
            if (current_A < min_A) {
                min_A = current_A;
            }
        }
        E[j] = min_A / (1.0 - a[j + 1]);
    }

    cout << fixed << setprecision(15) << E[0] << "\n";

    return 0;
}
