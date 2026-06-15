#include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    vector<int> h(n);
    
    int total_height = 0, total_diff = 0;
    for (int i = 0; i < n; i++) {
        cin >> h[i];
        total_height += h[i];
    }
    
    for (int i = 1; i < n; i++) {
        total_diff += abs(h[i] - h[i-1]);
    }
    
    int original_perimeter = 2 * total_height + 2 + total_diff;
    int min_perimeter = (original_perimeter + 1) / 2;
    
    int best_sum = 0;
    vector<int> best_removed;
    
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<int> kept;
        int removed_sum = 0;
        vector<int> removed_indices;
        
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                kept.push_back(i);
            } else {
                removed_sum += h[i];
                removed_indices.push_back(i + 1);
            }
        }
        
        int sum_h = 0, sum_d = 0;
        for (int i = 0; i < kept.size(); i++) {
            sum_h += h[kept[i]];
        }
        for (int i = 1; i < kept.size(); i++) {
            sum_d += abs(h[kept[i]] - h[kept[i-1]]);
        }
        
        int remaining = 2 * sum_h + 2 + sum_d;
        
        if (remaining >= min_perimeter && removed_sum > best_sum) {
            best_sum = removed_sum;
            best_removed = removed_indices;
        }
    }
    
    cout << best_sum << "\n" << best_removed.size() << "\n";
    for (int x : best_removed) cout << x << " ";
    
    return 0;
}
