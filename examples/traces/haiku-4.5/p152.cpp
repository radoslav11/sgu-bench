#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<long long> votes(n);
    long long total = 0;
    
    for(int i = 0; i < n; i++){
        cin >> votes[i];
        total += votes[i];
    }
    
    // Calculate exact percentages
    vector<double> exact(n);
    for(int i = 0; i < n; i++){
        exact[i] = 100.0 * votes[i] / total;
    }
    
    // Start with floor values
    vector<int> result(n);
    int sum = 0;
    vector<pair<double, int>> remainders; // (fractional part, index)
    
    for(int i = 0; i < n; i++){
        result[i] = (int)floor(exact[i]);
        sum += result[i];
        remainders.push_back({exact[i] - floor(exact[i]), i});
    }
    
    // Distribute remaining percentage points to candidates with largest remainders
    int remaining = 100 - sum;
    
    // Sort by remainder in descending order
    sort(remainders.begin(), remainders.end(), greater<pair<double, int>>());
    
    for(int i = 0; i < remaining; i++){
        result[remainders[i].second]++;
    }
    
    // Verify solution: each value should be floor or ceil of exact percentage
    for(int i = 0; i < n; i++){
        int floor_val = (int)floor(exact[i]);
        int ceil_val = (int)ceil(exact[i]);
        if(result[i] != floor_val && result[i] != ceil_val){
            cout << "No solution" << endl;
            return 0;
        }
    }
    
    // Verify sum
    int final_sum = 0;
    for(int i = 0; i < n; i++){
        final_sum += result[i];
    }
    
    if(final_sum != 100){
        cout << "No solution" << endl;
        return 0;
    }
    
    for(int i = 0; i < n; i++){
        if(i > 0) cout << " ";
        cout << result[i];
    }
    cout << endl;
    
    return 0;
}
