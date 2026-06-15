#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, x;
    cin >> N >> x;
    
    vector<set<int>> friends(N + 1);
    
    for (int i = 1; i <= N; i++) {
        int d;
        cin >> d;
        for (int j = 0; j < d; j++) {
            int f;
            cin >> f;
            friends[i].insert(f);
        }
    }
    
    set<int> fof;
    
    // For each friend b of x
    for (int b : friends[x]) {
        // For each friend c of b
        for (int c : friends[b]) {
            // c is not x, not a friend of x
            if (c != x && friends[x].find(c) == friends[x].end()) {
                fof.insert(c);
            }
        }
    }
    
    cout << fof.size() << "\n";
    for (int c : fof) {
        cout << c << " ";
    }
    if (!fof.empty()) cout << "\n";
    
    return 0;
}
