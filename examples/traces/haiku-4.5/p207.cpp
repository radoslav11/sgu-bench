#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, Y;
    cin >> N >> M >> Y;
    
    vector<int> X(N);
    for(int i = 0; i < N; i++){
        cin >> X[i];
    }
    
    // Calculate base allocation (floor of ideal amount)
    vector<long long> K(N);
    long long total = 0;
    vector<pair<double, int>> fractions; // (fractional part, -index for tie-breaking)
    
    for(int i = 0; i < N; i++){
        long long ideal = (long long)X[i] * M / Y;
        K[i] = ideal;
        total += K[i];
        
        // Calculate fractional part
        long long remainder = ((long long)X[i] * M) % Y;
        double frac = (double)remainder / Y;
        fractions.push_back({frac, -i});
    }
    
    // Distribute remaining coins to those with largest fractional parts
    long long remaining = M - total;
    
    // Sort by fractional part in descending order
    sort(fractions.rbegin(), fractions.rend());
    
    for(int i = 0; i < remaining; i++){
        K[-fractions[i].second]++;
    }
    
    // Output result
    for(int i = 0; i < N; i++){
        if(i > 0) cout << " ";
        cout << K[i];
    }
    cout << "\n";
    
    return 0;
}
