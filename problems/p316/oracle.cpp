#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    cin >> N >> M;

    vector<int> hp(N, 100);
    vector<int> score(N, 0);

    for (int i = 0; i < M; i++) {
        int s, t;
        cin >> s >> t;
        s--;
        t--;

        if (hp[t] > 0) {
            score[s] += 3;
        }
        hp[t] -= 8;
    }

    for (int i = 0; i < N; i++) {
        if (hp[i] > 0) {
            score[i] += hp[i] / 2;
        }
    }

    for (int i = 0; i < N; i++) {
        cout << hp[i] << " " << score[i] << "\n";
    }

    return 0;
}
