#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<pair<double, double>> m(n);
    for(int i = 0; i < n; i++){
        cin >> m[i].first >> m[i].second;
    }
    
    // Try to construct vertices starting with V_0 = (0, 0)
    // Then compute all vertices and check if V_0 = 2*M_{n-1} - V_{n-1}
    
    // Actually, we need to solve for V_0
    // Using the constraint that it's a closed polygon
    
    // Vertices alternate in sign when we unroll
    // V_0 + V_2 + V_4 + ... = ... (sum involving midpoints)
    
    // Let me use a different approach:
    // Express V_i in terms of V_0 and midpoints, then solve
    
    vector<double> coeff(n); // coefficients for V_0 in each V_i
    coeff[0] = 1.0;
    
    for(int i = 1; i < n; i++){
        coeff[i] = -coeff[i-1];
    }
    
    // V_i = coeff[i] * V_0 + (linear combination of midpoints)
    // Compute the constant term for each vertex
    
    vector<pair<double, double>> const_term(n);
    const_term[0] = {0, 0};
    
    for(int i = 1; i < n; i++){
        const_term[i] = {2*m[i-1].first - const_term[i-1].first,
                         2*m[i-1].second - const_term[i-1].second};
    }
    
    // Closure: V_0 = 2*M_{n-1} - V_{n-1}
    // V_0 = 2*M_{n-1} - (coeff[n-1]*V_0 + const_term[n-1])
    // V_0 + coeff[n-1]*V_0 = 2*M_{n-1} - const_term[n-1]
    // V_0 * (1 + coeff[n-1]) = 2*M_{n-1} - const_term[n-1]
    
    double denom = 1.0 + coeff[n-1];
    
    if(abs(denom) < 1e-9){
        cout << "NO\n";
        return 0;
    }
    
    pair<double, double> v0;
    v0.first = (2*m[n-1].first - const_term[n-1].first) / denom;
    v0.second = (2*m[n-1].second - const_term[n-1].second) / denom;
    
    // Construct all vertices
    vector<pair<double, double>> v(n);
    v[0] = v0;
    
    for(int i = 1; i < n; i++){
        v[i] = {2*m[i-1].first - v[i-1].first, 2*m[i-1].second - v[i-1].second};
    }
    
    // Verify closure
    pair<double, double> check = {2*m[n-1].first - v[n-1].first, 2*m[n-1].second - v[n-1].second};
    
    if(abs(check.first - v[0].first) > 1e-6 || abs(check.second - v[0].second) > 1e-6){
        cout << "NO\n";
        return 0;
    }
    
    cout << "YES\n";
    cout << fixed << setprecision(3);
    for(int i = 0; i < n; i++){
        cout << v[i].first << " " << v[i].second << "\n";
    }
    
    return 0;
}
