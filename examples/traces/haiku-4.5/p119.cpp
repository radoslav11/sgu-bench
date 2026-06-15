#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, A0, B0;
    cin >> N >> A0 >> B0;
    
    vector<pair<int, int>> valid_pairs;
    
    // For each scalar t from 0 to N-1
    for (int t = 0; t < N; t++) {
        int A = (t * A0) % N;
        int B = (t * B0) % N;
        valid_pairs.push_back({A, B});
    }
    
    // Sort pairs
    sort(valid_pairs.begin(), valid_pairs.end());
    
    // Remove duplicates
    valid_pairs.erase(unique(valid_pairs.begin(), valid_pairs.end()), valid_pairs.end());
    
    cout << valid_pairs.size() << "\n";
    for (auto p : valid_pairs) {
        cout << p.first << " " << p.second << "\n";
    }
    
    return 0;
}
