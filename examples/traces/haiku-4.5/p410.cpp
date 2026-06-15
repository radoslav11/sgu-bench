#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<long long> a(n);
    for(int i = 0; i < n; i++){
        cin >> a[i];
    }
    
    vector<string> operations;
    
    while(true){
        // Check if all are 0
        bool all_zero = true;
        for(int i = 0; i < n; i++){
            if(a[i] > 0){
                all_zero = false;
                break;
            }
        }
        if(all_zero) break;
        
        // Check if any is 0
        bool has_zero = false;
        for(int i = 0; i < n; i++){
            if(a[i] == 0){
                has_zero = true;
                break;
            }
        }
        
        if(has_zero){
            // Clone the planet with minimum
            int min_idx = 0;
            for(int i = 1; i < n; i++){
                if(a[i] < a[min_idx]){
                    min_idx = i;
                }
            }
            a[min_idx] *= 2;
            operations.push_back("science mission to the planet " + to_string(min_idx + 1));
        } else {
            // Flying mission
            for(int i = 0; i < n; i++){
                a[i]--;
            }
            operations.push_back("flying mission");
        }
    }
    
    cout << operations.size() << "\n";
    if(operations.size() <= 1000){
        for(const auto& op : operations){
            cout << op << "\n";
        }
    }
    
    return 0;
}
