#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, x;
    if (!(cin >> N >> x)) return 0;
    
    vector<vector<int>> adj(N + 1);
    for (int i = 1; i <= N; ++i) {
        int d;
        cin >> d;
        for (int j = 0; j < d; ++j) {
            int f;
            cin >> f;
            adj[i].push_back(f);
        }
    }
    
    vector<bool> is_friend(N + 1, false);
    for (int f : adj[x]) {
        is_friend[f] = true;
    }
    
    vector<bool> is_fof(N + 1, false);
    for (int f : adj[x]) {
        for (int ff : adj[f]) {
            if (ff != x && !is_friend[ff]) {
                is_fof[ff] = true;
            }
        }
    }
    
    vector<int> result;
    for (int i = 1; i <= N; ++i) {
        if (is_fof[i]) {
            result.push_back(i);
        }
    }
    
    cout << result.size() << "\n";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 == result.size() ? "" : " ");
    }
    cout << "\n";
    
    return 0;
}
