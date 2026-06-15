#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    
    vector<int> scores(n, 0);
    
    for (int match = 0; match < m; match++) {
        int a, b;
        cin >> a >> b;
        
        for (int participant = 0; participant < n; participant++) {
            int pred_a, pred_b;
            cin >> pred_a >> pred_b;
            
            // Check winner/tie
            int actual_result = (a > b) ? 1 : (a < b) ? -1 : 0;
            int pred_result = (pred_a > pred_b) ? 1 : (pred_a < pred_b) ? -1 : 0;
            
            if (actual_result == pred_result) {
                scores[participant] += 2;
            }
            
            // Check difference
            if (a - b == pred_a - pred_b) {
                scores[participant] += 3;
            }
            
            // Check first team goals
            if (a == pred_a) {
                scores[participant] += 1;
            }
            
            // Check second team goals
            if (b == pred_b) {
                scores[participant] += 1;
            }
        }
    }
    
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << " ";
        cout << scores[i];
    }
    cout << endl;
    
    return 0;
}
