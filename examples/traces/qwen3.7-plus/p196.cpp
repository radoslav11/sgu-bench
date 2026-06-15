#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    if (cin >> n >> m) {
        vector<int> deg(n + 1, 0);
        for (int i = 0; i < 2 * m; ++i) {
            int u;
            cin >> u;
            deg[u]++;
        }
        
        long long sum_squares = 0;
        for (int i = 1; i <= n; ++i) {
            sum_squares += 1LL * deg[i] * deg[i];
        }
        
        cout << sum_squares << "\n";
    }
    
    return 0;
}
