#include <iostream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int k;
    if (cin >> k) {
        long long f[45] = {0, 1, 1};
        long long sum = 0;
        for (int i = 1; i <= k; ++i) {
            if (i >= 3) {
                f[i] = f[i-1] + f[i-2];
            }
            sum += f[i];
        }
        cout << sum << "\n";
    }
    
    return 0;
}
