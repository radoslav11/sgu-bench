#include<bits/stdc++.h>
using namespace std main;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<double> x(n);
    for(int i = 0; i < n; i++){
        cin >> x[i];
    }
    
    double best_cost = 1e18;
    double best_d = 0;
    double best_a = 0;
    
    // Try different spacing values
    // The optimal d is likely around (x[n-1] - x[0]) / (n-1)
    double d_center = (x[n-1] - x[0]) / (n-1);
    
    // Search around this value
    for(double d = d_center * 0.5; d <= d_center * 1.5; d += d_center * 0.001){
        if(d <= 0) continue;
        
        // For this d, find optimal a
        vector<double> offsets(n);
        for(int i = 0; i < n; i++){
            offsets[i] = x[i] - (i * d);
        }
        sort(offsets.begin(), offsets.end());
        double a = offsets[n/2]; // median
        
        // Calculate cost
        double cost = 0;
        for(int i = 0; i < n; i++){
            cost += abs(x[i] - (a + i * d));
        }
        
        if(cost < best_cost){
            best_cost = cost;
            best_d = d;
            best_a = a;
        }
    }
    
    // Fine-tune around best_d
    for(double d = best_d * 0.99; d <= best_d * 1.01; d += best_d * 0.0001){
        if(d <= 0) continue;
        
        vector<double> offsets(n);
        for(int i = 0; i < n; i++){
            offsets[i] = x[i] - (i * d);
        }
        sort(offsets.begin(), offsets.end());
        double a = offsets[n/2];
        
        double cost = 0;
        for(int i = 0; i < n; i++){
            cost += abs(x[i] - (a + i * d));
        }
        
        if(cost < best_cost){
            best_cost = cost;
            best_d = d;
            best_a = a;
        }
    }
    
    cout << fixed << setprecision(4) << best_cost << "\n";
    cout << fixed << setprecision(10);
    for(int i = 0; i < n; i++){
        if(i > 0) cout << " ";
        cout << (best_a + i * best_d);
    }
    cout << "\n";
    
    return 0;
}
