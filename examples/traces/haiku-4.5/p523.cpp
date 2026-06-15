#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, f;
    cin >> n >> f;
    
    vector<int> buttons(n);
    set<int> highlighted;
    
    for (int i = 0; i < n; i++) {
        cin >> buttons[i];
        highlighted.insert(buttons[i]);
    }
    
    int current = f;
    vector<int> result;
    
    while (!highlighted.empty()) {
        // Get the first (earliest pressed) highlighted button
        int target = -1;
        for (int i = 0; i < n; i++) {
            if (highlighted.count(buttons[i])) {
                target = buttons[i];
                break;
            }
        }
        
        // Find all highlighted floors between current and target
        vector<int> between;
        if (target > current) {
            for (int floor : highlighted) {
                if (floor > current && floor < target) {
                    between.push_back(floor);
                }
            }
            sort(between.begin(), between.end());
        } else {
            for (int floor : highlighted) {
                if (floor < current && floor > target) {
                    between.push_back(floor);
                }
            }
            sort(between.rbegin(), between.rend());
        }
        
        // Visit floors between current and target
        for (int floor : between) {
            result.push_back(floor);
            highlighted.erase(floor);
        }
        
        // Visit target
        result.push_back(target);
        highlighted.erase(target);
        current = target;
    }
    
    for (int floor : result) {
        cout << floor << " ";
    }
    cout << "\n";
    
    return 0;
}
