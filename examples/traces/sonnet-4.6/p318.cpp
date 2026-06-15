#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    
    // For each resource, which users need it
    vector<set<int>> demand(N + 1);
    
    for (int i = 1; i <= M; i++) {
        int sz;
        cin >> sz;
        for (int j = 0; j < sz; j++) {
            int r;
            cin >> r;
            demand[r].insert(i);
        }
    }
    
    set<set<int>> distinct_sets;
    for (int r = 1; r <= N; r++) {
        if (!demand[r].empty()) {
            distinct_sets.insert(demand[r]);
        }
    }
    
    cout << distinct_sets.size() << endl;
    
    return 0;
}
