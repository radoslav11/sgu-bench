#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<string> phrases(M);
    for (int i = 0; i < M; ++i) {
        cin >> phrases[i];
    }

    cout << phrases[(N - 1) % M] << '\n';

    return 0;
}
