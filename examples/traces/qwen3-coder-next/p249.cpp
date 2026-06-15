#include <iostream>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    
    int rows = 1 << N;
    int cols = 1 << M;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int col_gray = j ^ (j >> 1);
            int row_gray = i ^ (i >> 1);
            int val = (col_gray << N) | row_gray;
            if (j > 0) {
                cout << " ";
            }
            cout << val;
        }
        cout << "\n";
    }
    
    return 0;
}
