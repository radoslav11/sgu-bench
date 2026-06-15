#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<int> hp(N, 100);
    vector<int> score(N, 0);

    for (int i = 0; i < M; ++i) {
        int shooter, target;
        cin >> shooter >> target;
        --shooter;
        --target;

        if (hp[target] > 0) {
            score[shooter] += 3;
        }

        hp[target] -= 8;
    }

    for (int i = 0; i < N; ++i) {
        if (hp[i] > 0) {
            score[i] += hp[i] / 2;
        }
        cout << hp[i] << ' ' << score[i] << '\n';
    }

    return 0;
}
