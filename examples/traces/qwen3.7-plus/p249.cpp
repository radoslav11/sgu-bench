#include <iostream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    
    int rows = 1 << n;
    int cols = 1 << m;
    
    for (int i = 0; i < rows; ++i) {
        int a = i ^ (i >> 1);
        for (int j = 0; j < cols; ++j) {
            int b = j ^ (j >> 1);
            int val = a | (b << n);
            if (j > 0) cout << ' ';
            cout << val;
        }
        cout << '\n';
    }
    
    return 0;
}
