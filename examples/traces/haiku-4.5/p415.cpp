#include<bits/stdc++.h>
using std::cin, std::cout;

int n, x;
vector<int> coins;
set<set<int>> validSubsets;

void findSubsets(int idx, int currentSum, set<int>& currentSubset) {
    if (currentSum == x) {
        validSubsets.insert(currentSubset);
        return;
    }
    if (currentSum > x || idx == n) {
        return;
    }
    
    // Include current coin
    currentSubset.insert(coins[idx]);
    findSubsets(idx + 1, currentSum + coins[idx], currentSubset);
    currentSubset.erase(coins[idx]);
    
    // Exclude current coin
    findSubsets(idx + 1, currentSum, currentSubset);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> x;
    coins.resize(n);
    
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }
    
    set<int> currentSubset;
    findSubsets(0, 0, currentSubset);
    
    // Find intersection of all valid subsets
    set<int> necessary;
    if (!validSubsets.empty()) {
        necessary = *validSubsets.begin();
        for (const auto& subset : validSubsets) {
            set<int> temp;
            set_intersection(necessary.begin(), necessary.end(),
                           subset.begin(), subset.end(),
                           inserter(temp, temp.begin()));
            necessary = temp;
        }
    }
    
    cout << necessary.size() << "\n";
    for (int coin : necessary) {
        cout << coin << " ";
    }
    if (necessary.size() > 0) {
        cout << "\n";
    }
    
    return 0;
}
