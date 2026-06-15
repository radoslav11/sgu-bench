#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    
    if (n == 0) {
        cout << 0 << '\n';
        return 0;
    }
    
    vector<int> b = a;
    sort(b.begin(), b.end());
    
    if (a == b) {
        cout << 0 << '\n';
        return 0;
    }
    
    vector<vector<pair<int, int>>> all_steps;
    vector<int> current = a;
    
    while (true) {
        vector<int> target_sorted = current;
        sort(target_sorted.begin(), target_sorted.end());
        
        if (current == target_sorted) {
            break;
        }
        
        map<int, queue<int>> positions;
        for (int i = 0; i < n; i++) {
            positions[target_sorted[i]].push(i);
        }
        
        vector<int> p(n);
        for (int i = 0; i < n; i++) {
            p[i] = positions[current[i]].front();
            positions[current[i]].pop();
        }
        
        vector<bool> visited(n, false);
        vector<vector<int>> cycles;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                vector<int> cycle;
                int j = i;
                while (!visited[j]) {
                    visited[j] = true;
                    cycle.push_back(j);
                    j = p[j];
                }
                if (cycle.size() > 1) {
                    cycles.push_back(cycle);
                }
            }
        }
        
        vector<pair<int, int>> step_pairs;
        for (const auto& cycle : cycles) {
            int len = cycle.size();
            for (int i = 0; i < len / 2; i++) {
                step_pairs.push_back({cycle[i], cycle[len - 1 - i]});
            }
        }
        
        if (step_pairs.empty()) {
            break;
        }
        
        for (auto& [i, j] : step_pairs) {
            swap(current[i], current[j]);
        }
        all_steps.push_back(step_pairs);
    }
    
    cout << (int)all_steps.size() << '\n';
    for (const auto& step : all_steps) {
        cout << step.size();
        for (const auto& [i, j] : step) {
            cout << " " << i + 1 << " " << j + 1;
        }
        cout << '\n';
    }
    
    return 0;
}
