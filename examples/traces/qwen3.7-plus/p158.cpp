#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    double L;
    int M;
    if (!(cin >> L >> M)) return 0;

    vector<double> P(M);
    for (int i = 0; i < M; ++i) {
        cin >> P[i];
    }

    int N;
    cin >> N;

    vector<double> D(N);
    D[0] = 0.0;
    for (int i = 1; i < N; ++i) {
        cin >> D[i];
    }

    vector<double> candidates;
    candidates.push_back(0.0);
    candidates.push_back(L - D[N - 1]);

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N - 1; ++j) {
            double S = P[i] - (D[j] + D[j + 1]) / 2.0;
            if (S >= -1e-9 && S <= L - D[N - 1] + 1e-9) {
                S = max(0.0, min(L - D[N - 1], S));
                candidates.push_back(S);
            }
        }
    }

    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end(), [](double a, double b) {
        return abs(a - b) < 1e-9;
    }), candidates.end());

    double best_S = 0.0;
    double best_sum = -1.0;

    for (double S : candidates) {
        double total_dist = 0;
        int j = 0;
        for (int i = 0; i < M; ++i) {
            while (j + 1 < N && fabs(D[j + 1] + S - P[i]) < fabs(D[j] + S - P[i])) {
                j++;
            }
            total_dist += fabs(D[j] + S - P[i]);
        }
        if (total_dist > best_sum) {
            best_sum = total_dist;
            best_S = S;
        }
    }

    cout << best_S << " " << best_sum << "\n";

    return 0;
}
