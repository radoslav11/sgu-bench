#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, f;
    cin >> n >> f;
    
    vector<int> e(n);
    for (int i = 0; i < n; i++) cin >> e[i];
    
    // highlighted[floor] = press order (1-indexed), 0 means not highlighted
    vector<int> pressOrder(101, 0);
    for (int i = 0; i < n; i++) {
        pressOrder[e[i]] = i + 1;
    }
    
    int current = f;
    vector<int> result;
    
    // set of highlighted buttons
    set<int> highlighted;
    for (int i = 0; i < n; i++) highlighted.insert(e[i]);
    
    while (!highlighted.empty()) {
        // Find a: highlighted button with smallest press order
        int a = -1;
        int bestOrder = INT_MAX;
        for (int fl : highlighted) {
            if (pressOrder[fl] < bestOrder) {
                bestOrder = pressOrder[fl];
                a = fl;
            }
        }
        
        // Move from current toward a
        int dir = (a > current) ? 1 : -1;
        int pos = current;
        while (pos != a) {
            pos += dir;
            if (highlighted.count(pos)) {
                highlighted.erase(pos);
                result.push_back(pos);
                if (pos == a) break;
            }
        }
        // Make sure a is removed (it should be by now)
        // a is always visited when we reach it
        current = a;
    }
    
    for (int i = 0; i < (int)result.size(); i++) {
        if (i) cout << " ";
        cout << result[i];
    }
    cout << endl;
    
    return 0;
}
