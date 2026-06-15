#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<string> seq_lines(n);
    vector<set<int>> seq_sets(n);
    
    // Read sequences
    for (int i = 0; i < n; i++) {
        int k;
        cin >> k;
        seq_lines[i] = to_string(k);
        for (int j = 0; j < k; j++) {
            int x;
            cin >> x;
            seq_lines[i] += " " + to_string(x);
            seq_sets[i].insert(x);
        }
    }
    
    // Process queries
    for (int q = 0; q < m; q++) {
        int l;
        cin >> l;
        vector<int> must_have, must_not;
        for (int j = 0; j < l; j++) {
            int b;
            cin >> b;
            if (b > 0) must_have.push_back(b);
            else must_not.push_back(-b);
        }
        
        vector<int> results;
        for (int i = 0; i < n; i++) {
            bool ok = true;
            for (int v : must_have) {
                if (seq_sets[i].find(v) == seq_sets[i].end()) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                for (int v : must_not) {
                    if (seq_sets[i].find(v) != seq_sets[i].end()) {
                        ok = false;
                        break;
                    }
                }
            }
            if (ok) results.push_back(i);
        }
        
        cout << results.size() << "\n";
        for (int idx : results) {
            cout << seq_lines[idx] << "\n";
        }
    }
    
    return 0;
}
