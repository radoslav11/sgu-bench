#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Tea {
    int cost;
    int type;
    int idx;
};

int main() {
    int K, N;
    cin >> K >> N;
    
    vector<Tea> green, black;
    
    for (int i = 0; i < N; i++) {
        int c, s;
        cin >> c >> s;
        Tea tea = {c, s, i + 1};
        if (s == 0) {
            green.push_back(tea);
        } else {
            black.push_back(tea);
        }
    }
    
    sort(green.begin(), green.end(), [](const Tea& a, const Tea& b) {
        return a.cost < b.cost;
    });
    
    sort(black.begin(), black.end(), [](const Tea& a, const Tea& b) {
        return a.cost < b.cost;
    });
    
    // Precompute prefix sums
    vector<long long> green_prefix(green.size() + 1, 0);
    for (int i = 0; i < green.size(); i++) {
        green_prefix[i + 1] = green_prefix[i] + green[i].cost;
    }
    
    vector<long long> black_prefix(black.size() + 1, 0);
    for (int i = 0; i < black.size(); i++) {
        black_prefix[i + 1] = black_prefix[i] + black[i].cost;
    }
    
    long long min_cost = LLONG_MAX;
    int best_g = -1, best_b = -1;
    
    // Try all possible g (number of green teas to use)
    for (int g = 0; g <= min(K, (int)green.size()); g++) {
        int b = K - g;
        if (b < 0 || b > (int)black.size()) continue;
        
        // Check constraint: no three consecutive same kind
        if (max(g, b) <= 2 * min(g, b) + 2) {
            long long cost = green_prefix[g] + black_prefix[b];
            if (cost < min_cost) {
                min_cost = cost;
                best_g = g;
                best_b = b;
            }
        }
    }
    
    if (best_g == -1) {
        cout << "Impossible" << endl;
        return 0;
    }
    
    // Extract the selected teas
    vector<Tea> selected_green(green.begin(), green.begin() + best_g);
    vector<Tea> selected_black(black.begin(), black.begin() + best_b);
    
    // Arrange them to avoid 3 consecutive same kind
    vector<Tea> result;
    int g_idx = 0, b_idx = 0;
    
    while (result.size() < K) {
        // If last two are same type, we must choose the other type
        if (result.size() >= 2 && 
            result[result.size() - 1].type == result[result.size() - 2].type) {
            int last_type = result[result.size() - 1].type;
            if (last_type == 0) { // last is green, need black
                if (b_idx < (int)selected_black.size()) {
                    result.push_back(selected_black[b_idx++]);
                } else {
                    // This should not happen if constraint is satisfied
                    result.push_back(selected_green[g_idx++]);
                }
            } else { // last is black, need green
                if (g_idx < (int)selected_green.size()) {
                    result.push_back(selected_green[g_idx++]);
                } else {
                    // This should not happen if constraint is satisfied
                    result.push_back(selected_black[b_idx++]);
                }
            }
        } else {
            // Choose the type with more remaining, or if equal, prefer any
            if (g_idx < (int)selected_green.size() && 
                (b_idx >= (int)selected_black.size() || 
                 (int)selected_green.size() - g_idx >= (int)selected_black.size() - b_idx)) {
                result.push_back(selected_green[g_idx++]);
            } else if (b_idx < (int)selected_black.size()) {
                result.push_back(selected_black[b_idx++]);
            } else {
                // This should not happen since g + b = K
                break;
            }
        }
    }
    
    // Output the indices
    for (int i = 0; i < K; i++) {
        if (i > 0) cout << " ";
        cout << result[i].idx;
    }
    cout << endl;
    
    return 0;
}
