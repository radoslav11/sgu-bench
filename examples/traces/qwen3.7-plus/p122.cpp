#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <bitset>

using namespace std;

const int MAXN = 1005;
bitset<MAXN> adj[MAXN];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;
    
    string line;
    getline(cin, line);
    int count = 0;
    while (count < n && getline(cin, line)) {
        bool empty = true;
        for (char c : line) {
            if (!isspace(c)) {
                empty = false;
                break;
            }
        }
        if (empty) continue;
        
        count++;
        stringstream ss(line);
        int f;
        while (ss >> f) {
            adj[count].set(f);
            adj[f].set(count);
        }
    }

    if (n == 1) {
        cout << "1 1\n";
        return 0;
    }

    vector<int> P;
    P.push_back(1);
    vector<bool> visited(n + 1, false);
    visited[1] = true;
    int k = 1;

    while (k < n) {
        bool extended = false;
        
        // Try to extend from front
        for (int u = 1; u <= n; u++) {
            if (!visited[u] && adj[P[0]].test(u)) {
                P.insert(P.begin(), u);
                visited[u] = true;
                k++;
                extended = true;
                break;
            }
        }
        if (extended) continue;
        
        // Try to extend from back
        for (int u = 1; u <= n; u++) {
            if (!visited[u] && adj[P[k-1]].test(u)) {
                P.push_back(u);
                visited[u] = true;
                k++;
                extended = true;
                break;
            }
        }
        if (extended) continue;
        
        // Form a cycle
        int found_i = -1;
        for (int i = 0; i < k - 1; i++) {
            if (adj[P[i]].test(P[k-1]) && adj[P[i+1]].test(P[0])) {
                found_i = i;
                break;
            }
        }
        
        if (found_i != -1) {
            reverse(P.begin() + found_i + 1, P.end());
            
            int m = -1;
            int u_out = -1;
            for (int j = 0; j < k; j++) {
                for (int u = 1; u <= n; u++) {
                    if (!visited[u] && adj[P[j]].test(u)) {
                        m = j;
                        u_out = u;
                        break;
                    }
                }
                if (m != -1) break;
            }
            
            if (m != -1) {
                rotate(P.begin(), P.begin() + m, P.end());
                P.insert(P.begin(), u_out);
                visited[u_out] = true;
                k++;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    if (k < n) {
        cout << "No solution\n";
        return 0;
    }

    int idx = -1;
    for (int i = 0; i < n; i++) {
        if (P[i] == 1) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "No solution\n";
        return 0;
    }

    rotate(P.begin(), P.begin() + idx, P.end());

    if (!adj[P.back()].test(1)) {
        cout << "No solution\n";
        return 0;
    }

    for (int i = 0; i < n - 1; i++) {
        if (!adj[P[i]].test(P[i+1])) {
            cout << "No solution\n";
            return 0;
        }
    }

    P.push_back(1);
    for (int i = 0; i <= n; i++) {
        cout << P[i] << (i == n ? "" : " ");
    }
    cout << "\n";

    return 0;
}
