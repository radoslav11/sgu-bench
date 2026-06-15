#include <bits/stdc++.h>
using namespace std;
int H, W;
int B[305][305];
int A[305][305];

bool solve(int a00, int a10) {
    A[0][0] = a00; A[1][0] = a10;
    // fill row 0 and row 1 col by col using B[0][j]
    for (int j = 0; j < W-1; j++) {
        int val = B[0][j] - A[0][j] - A[1][j];
        // A[0][j+1] + A[1][j+1] = val, but need more...
        // can't determine individually
    }
    return false;
}

int main() {
    cin >> H >> W;
    for (int i = 0; i < H-1; i++) {
        string s; cin >> s;
        for (int j = 0; j < W-1; j++) B[i][j] = s[j]-'0';
    }
    for (int a00 = 0; a00 <= 1; a00++)
    for (int a01 = 0; a01 <= 1; a01++)
    for (int a10 = 0; a10 <= 1; a10++) {
        A[0][0]=a00; A[0][1]=a01; A[1][0]=a10;
        bool ok = true;
        // fill row 0
        for (int j = 1; j < W-1 && ok; j++) {
            // B[0][j-1] = A[0][j-1]+A[1][j-1]+A[0][j]+A[1][j]
            // need A[1][j-1] known first
            // Actually determine col by col with first col known
        }
        // Try differently: fix row0[0], row0[1], row1[0]
        // row1[1] = B[0][0] - A[0][0] - A[0][1] - A[1][0]
        A[1][1] = B[0][0] - A[0][0] - A[0][1] - A[1][0];
        if (A[1][1] < 0 || A[1][1] > 1) { ok = false; }
        // fill rest of row 1: A[1][j+1] = B[0][j] - A[0][j] - A[0][j+1] - A[1][j]
        // but need A[0][j] for j>1
        // Hmm, so need full row 0
    }
    cout << "CORRUPT\n";
}
