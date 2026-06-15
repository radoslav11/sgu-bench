#include <iostream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <set>

using namespace std;

// Check if a sequence contains the required numbers and excludes the forbidden numbers
bool matches(const vector<int>& seq, const vector<int>& query) {
    set<int> required;
    set<int> forbidden;
    
    for (int x : query) {
        if (x > 0) {
            required.insert(x);
        } else {
            forbidden.insert(-x);
        }
    }
    
    // Check if all required numbers are present
    for (int req : required) {
        bool found = false;
        for (int num : seq) {
            if (num == req) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    
    // Check if no forbidden numbers are present
    for (int forb : forbidden) {
        for (int num : seq) {
            if (num == forb) {
                return false;
            }
        }
    }
    
    return true;
}

int main() {
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> sequences;
    for (int i = 0; i < n; i++) {
        int k;
        cin >> k;
        vector<int> seq(k);
        for (int j = 0; j < k; j++) {
            cin >> seq[j];
        }
        sequences.push_back(seq);
    }
    
    // Process each query
    for (int i = 0; i < m; i++) {
        int l;
        cin >> l;
        vector<int> query(l);
        for (int j = 0; j < l; j++) {
            cin >> query[j];
        }
        
        vector<vector<int>> matched;
        for (const auto& seq : sequences) {
            if (matches(seq, query)) {
                matched.push_back(seq);
            }
        }
        
        cout << matched.size() << endl;
        for (const auto& seq : matched) {
            cout << seq.size();
            for (int num : seq) {
                cout << " " << num;
            }
            cout << endl;
        }
    }
    
    return 0;
}
