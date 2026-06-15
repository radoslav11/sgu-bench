#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<int> a(n);
    for(int i = 0; i < n; i++){
        cin >> a[i];
    }
    
    int count = 0;
    
    // Try all 2^n configurations of mines in column 1
    for(int mask = 0; mask < (1 << n); mask++){
        // Check if this configuration is valid
        bool valid = true;
        
        for(int i = 0; i < n; i++){
            // Count mines in neighbors of cell (i, 1)
            int mines = 0;
            
            // Check cell (i-1, 0)
            if(i > 0 && (mask & (1 << (i-1)))){
                mines++;
            }
            // Check cell (i, 0)
            if(mask & (1 << i)){
                mines++;
            }
            // Check cell (i+1, 0)
            if(i < n-1 && (mask & (1 << (i+1)))){
                mines++;
            }
            
            // This should equal a[i]
            if(mines != a[i]){
                valid = false;
                break;
            }
        }
        
        if(valid){
            count++;
        }
    }
    
    cout << count << endl;
    
    return 0;
}
