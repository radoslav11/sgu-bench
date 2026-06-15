#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int K;
    cin >> K;

    while (K--) {
        long long N;
        int M;
        cin >> N >> M;

        vector<int> moves;
        moves.push_back(1);

        for (int i = 0; i < M; ++i) {
            int p;
            cin >> p;
            moves.push_back(p);
        }

        sort(moves.begin(), moves.end());
        moves.erase(unique(moves.begin(), moves.end()), moves.end());

        int L = moves.back();

        vector<char> win;
        win.push_back(1); // 0 matches: current player has already won, opponent took last

        vector<int> seen(1 << L, -1);

        bool answer = false;

        for (long long n = 1;; ++n) {
            bool cur = false;

            for (int mv : moves) {
                if (mv > n) break;
                if (!win[n - mv]) {
                    cur = true;
                    break;
                }
            }

            win.push_back(cur);

            if (n == N) {
                answer = cur;
                break;
            }

            if (n >= L) {
                int mask = 0;
                for (int i = 0; i < L; ++i) {
                    if (win[n - i]) mask |= (1 << i);
                }

                if (seen[mask] != -1) {
                    long long start = seen[mask];
                    long long period = n - start;
                    long long idx = start + (N - start) % period;
                    answer = win[idx];
                    break;
                }

                seen[mask] = n;
            }
        }

        if (answer)
            cout << "FIRST PLAYER MUST WIN\n";
        else
            cout << "SECOND PLAYER MUST WIN\n";
    }

    return 0;
}
