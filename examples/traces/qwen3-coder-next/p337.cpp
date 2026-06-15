#include <iostream>
#include <string>
using namespace std;

int main() {
    int K;
    string S;
    cin >> K;
    cin >> S;
    int n = S.length();
    string T = S + S;
    int max_m = (n - 1) / 2;

    for (int m = max_m; m >= 1; m--) {
        string best = "";
        int d = 0;
        for (int j = 0; j < m; j++) {
            if (T[j] != T[j + m]) {
                d++;
            }
        }

        if (d <= K) {
            best = T.substr(0, 2 * m);
        }

        for (int i = 1; i < n; i++) {
            if (T[i - 1] != T[i - 1 + m]) {
                d--;
            }
            if (T[i + m - 1] != T[i + 2 * m - 1]) {
                d++;
            }
            if (d <= K) {
                string candidate = T.substr(i, 2 * m);
                if (best.empty() || candidate < best) {
                    best = candidate;
                }
            }
        }

        if (!best.empty()) {
            cout << best << endl;
            return 0;
        }
    }

    cout << endl;
    return 0;
}
