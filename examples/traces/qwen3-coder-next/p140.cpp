#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    int N, P, B;
    cin >> N >> P >> B;
    vector<long long> A(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }

    vector<int> parent(P, -1);
    vector<int> add_coin(P, -1);
    queue<int> q;

    parent[0] = -2;
    q.push(0);

    while (!q.empty()) {
        int r = q.front();
        q.pop();
        for (int i = 0; i < N; i++) {
            int add_val = A[i] % P;
            int r2 = (r + add_val) % P;
            if (parent[r2] == -1) {
                parent[r2] = r;
                add_coin[r2] = i;
                q.push(r2);
            }
        }
    }

    if (parent[B] == -1) {
        cout << "NO" << endl;
    } else {
        cout << "YES" << endl;
        vector<int> X(N, 0);
        int r = B;
        while (r != 0) {
            int i = add_coin[r];
            X[i]++;
            r = parent[r];
        }
        for (int i = 0; i < N; i++) {
            if (i > 0) cout << " ";
            cout << X[i];
        }
        cout << endl;
    }

    return 0;
}
