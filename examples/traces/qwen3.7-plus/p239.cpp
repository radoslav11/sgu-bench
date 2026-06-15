#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (!(cin >> n)) return 0;
    
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    int valid_count = 0;
    for (int x1 = 0; x1 <= 1; ++x1) {
        vector<int> x(n + 2, 0);
        x[0] = 0;
        x[1] = x1;
        bool possible = true;
        for (int i = 1; i <= n; ++i) {
            x[i + 1] = a[i - 1] - x[i] - x[i - 1];
            if (x[i + 1] < 0 || x[i + 1] > 1) {
                possible = false;
                break;
            }
        }
        if (possible && x[n + 1] == 0) {
            valid_count++;
        }
    }

    cout << valid_count << "\n";
    return 0;
}
