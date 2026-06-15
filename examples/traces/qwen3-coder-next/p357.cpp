#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

int main() {
    vector<bool> functional(13, true);
    
    // Read first line: buttons 1, 2, 3, ↑
    for (int i = 0; i < 4; i++) {
        int x;
        cin >> x;
        functional[i] = (x == 1);
    }
    
    // Read second line: buttons 4, 5, 6, ↓
    for (int i = 0; i < 4; i++) {
        int x;
        cin >> x;
        functional[4 + i] = (x == 1);
    }
    
    // Read third line: buttons 7, 8, 9
    for (int i = 0; i < 3; i++) {
        int x;
        cin >> x;
        functional[8 + i] = (x == 1);
    }
    
    // Read fourth line: buttons --, 0
    for (int i = 0; i < 2; i++) {
        int x;
        cin >> x;
        functional[11 + i] = (x == 1);
    }
    
    // Read X and Y
    int X, Y;
    cin >> X >> Y;
    
    // BFS to find minimum clicks
    // State: current channel (0-99)
    vector<int> dist(100, INT_MAX);
    queue<int> q;
    
    dist[X] = 0;
    q.push(X);
    
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        
        // If we reached target, we can break early (BFS ensures minimum)
        if (cur == Y) break;
        
        // 1. Press numeric buttons (0-9) to go directly to that channel
        // Buttons mapping: 
        // 1->index 0, 2->1, 3->2, ↑->3
        // 4->4, 5->5, 6->6, ↓->7
        // 7->8, 8->9, 9->10
        // --->11, 0->12
        
        // Single digit channels (0-9)
        for (int digit = 0; digit <= 9; digit++) {
            if (digit == 0) {
                if (functional[12]) { // 0 button
                    if (dist[digit] > dist[cur] + 1) {
                        dist[digit] = dist[cur] + 1;
                        q.push(digit);
                    }
                }
            } else if (digit <= 3) {
                if (functional[digit - 1]) {
                    if (dist[digit] > dist[cur] + 1) {
                        dist[digit] = dist[cur] + 1;
                        q.push(digit);
                    }
                }
            } else if (digit <= 6) {
                if (functional[4 + digit - 4]) {
                    if (dist[digit] > dist[cur] + 1) {
                        dist[digit] = dist[cur] + 1;
                        q.push(digit);
                    }
                }
            } else if (digit <= 9) {
                if (functional[8 + digit - 7]) {
                    if (dist[digit] > dist[cur] + 1) {
                        dist[digit] = dist[cur] + 1;
                        q.push(digit);
                    }
                }
            }
        }
        
        // 2. Use two-digit inputs: press "--" then two digits
        if (functional[11]) { // "--" button works
            for (int d1 = 0; d1 <= 9; d1++) {
                for (int d2 = 0; d2 <= 9; d2++) {
                    // Get channel number from digits d1, d2 (d1 tens, d2 ones)
                    int target = d1 * 10 + d2;
                    
                    // Check if d1 and d2 buttons are functional
                    bool d1_ok = false, d2_ok = false;
                    if (d1 == 0) {
                        d1_ok = functional[12];
                    } else if (d1 <= 3) {
                        d1_ok = functional[d1 - 1];
                    } else if (d1 <= 6) {
                        d1_ok = functional[4 + d1 - 4];
                    } else if (d1 <= 9) {
                        d1_ok = functional[8 + d1 - 7];
                    }
                    
                    if (d2 == 0) {
                        d2_ok = functional[12];
                    } else if (d2 <= 3) {
                        d2_ok = functional[d2 - 1];
                    } else if (d2 <= 6) {
                        d2_ok = functional[4 + d2 - 4];
                    } else if (d2 <= 9) {
                        d2_ok = functional[8 + d2 - 7];
                    }
                    
                    if (d1_ok && d2_ok) {
                        if (dist[target] > dist[cur] + 3) { // "--" + d1 + d2
                            dist[target] = dist[cur] + 3;
                            q.push(target);
                        }
                    }
                }
            }
        }
        
        // 3. Press ↑ button: go to (cur+1) % 100
        if (functional[3]) { // ↑ works
            int next = (cur + 1) % 100;
            if (dist[next] > dist[cur] + 1) {
                dist[next] = dist[cur] + 1;
                q.push(next);
            }
        }
        
        // 4. Press ↓ button: go to (cur-1+100) % 100
        if (functional[7]) { // ↓ works
            int next = (cur - 1 + 100) % 100;
            if (dist[next] > dist[cur] + 1) {
                dist[next] = dist[cur] + 1;
                q.push(next);
            }
        }
    }
    
    if (dist[Y] == INT_MAX) {
        cout << -1 << endl;
    } else {
        cout << dist[Y] << endl;
    }
    
    return 0;
}
