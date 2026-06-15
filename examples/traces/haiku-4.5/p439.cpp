#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    string first, second;
    cin >> n >> m >> first >> second;
    
    // Find lexicographically smallest rotation of second tape
    string best_second = second;
    
    for(int i = 1; i < m; i++){
        string rotated = second.substr(i) + second.substr(0, i);
        if(rotated < best_second){
            best_second = rotated;
        }
    }
    
    // Now find the best rotation of first tape
    int best_first_shift = -1;
    int best_shift_count = INT_MAX;
    
    for(int shift = 0; shift < n; shift++){
        // Rotate first tape left by 'shift' positions
        string rotated_first = first.substr(shift) + first.substr(0, shift);
        
        // Check if first m characters match or differ by 1
        int diff_count = 0;
        for(int i = 0; i < m; i++){
            if(rotated_first[i] != best_second[i]){
                diff_count++;
            }
        }
        
        if(diff_count <= 1){
            // This is a valid solution
            int shift_count = min(shift, n - shift);
            bool is_left_shift = (shift <= n - shift);
            
            if(shift_count < best_shift_count || 
               (shift_count == best_shift_count && is_left_shift && shift < n - shift)){
                best_shift_count = shift_count;
                best_first_shift = shift;
            }
        }
    }
    
    // Apply the best shift to first tape
    string result_first = first.substr(best_first_shift) + first.substr(0, best_first_shift);
    
    cout << best_second << "\n";
    cout << result_first << "\n";
    
    return 0;
}
