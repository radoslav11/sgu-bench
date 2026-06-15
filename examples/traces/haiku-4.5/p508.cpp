#include <bits/stdc++.h>
using namespace std;

long long binomial(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    
    long long result = 1;
    for (int i = 0; i < k; i++) {
        result = result * (n - i) / (i + 1);
    }
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, l1, l2;
    double p;
    cin >> n >> l1 >> l2 >> p;
    
    int l = l1 + l2;
    p = p / 100.0;
    
    // Calculate posterior probabilities for each k
    vector<double> prob(n + 1, 0);
    double total = 0;
    
    for (int k = 0; k <= n; k++) {
        // k must be able to provide l1 black balls
        // and n-k must be able to provide l2 white balls
        if (k < l1 || n - k < l2) {
            prob[k] = 0;
        } else {
            prob[k] = (double)binomial(k, l1) * binomial(n - k, l2);
            total += prob[k];
        }
    }
    
    // Normalize
    for (int k = 0; k <= n; k++) {
        prob[k] /= total;
    }
    
    // Find best range [a, b]
    int best_a = 0, best_b = n;
    int best_width = n;
    
    for (int a = 0; a <= n; a++) {
        double cumulative = 0;
        for (int b = a; b <= n; b++) {
            cumulative += prob[b];
            if (cumulative >= p - 1e-9) {
                int width = b - a;
                if (width < best_width) {
                    best_width = width;
                    best_a = a;
                    best_b = b;
                }
                break;
            }
        }
    }
    
    cout << best_a << " " << best_b << endl;
    
    return 0;
}
