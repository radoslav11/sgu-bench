#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    cin >> N >> M;
    
    int rows = 1 << N;
    int cols = 1 << M;
    
    for (int r = 0; r < rows; r++) {
        int gray_r = r ^ (r >> 1);
        for (int c = 0; c < cols; c++) {
            int gray_c = c ^ (c >> 1);
            int val = (gray_r << M) | gray_c;
            if (c > 0) cout << ' ';
            cout << val;
        }
        cout << '\n';
    }
    
    return 0;
}
