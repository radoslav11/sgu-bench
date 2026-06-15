#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N;
    cin >> N;

    if (N == 1) {
        cout << 0 << endl;
        cout << 0 << endl;
        return 0;
    }

    int T = (N % 2 == 0) ? (N - 1) : N;
    cout << T << endl;

    vector<vector<int>> ans(N, vector<int>(N, 0));

    int M = N;
    int size = N - 1;
    if (N % 2 == 1) {
        M = N + 1;
        size = N;
    }

    vector<int> arr;
    for (int i = 1; i < M; i++) {
        arr.push_back(i);
    }

    for (int r = 0; r < T; r++) {
        if (arr[r] < N) {
            ans[0][arr[r]] = r + 1;
            ans[arr[r]][0] = r + 1;
        }

        int num_pairs = (size - 1) / 2;
        for (int i = 1; i <= num_pairs; i++) {
            int idx1 = (r + i) % size;
            int idx2 = (r + size - i) % size;
            int p1 = arr[idx1];
            int p2 = arr[idx2];
            if (p1 < N && p2 < N) {
                ans[p1][p2] = r + 1;
                ans[p2][p1] = r + 1;
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j > 0) 
                cout << " ";
            cout << ans[i][j];
        }
        cout << endl;
    }

    return 0;
}
