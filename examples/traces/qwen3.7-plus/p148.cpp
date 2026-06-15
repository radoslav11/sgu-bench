#pragma GCC optimize("O3")
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<int> W(N + 1), L(N + 1), P(N + 1);
    for (int i = 1; i <= N; ++i) {
        cin >> W[i] >> L[i] >> P[i];
    }

    int min_cost = -1;
    int best_start = -1;

    for (int i = 1; i <= N; ++i) {
        int current_cost = P[i];
        int current_water = W[i];
        
        for (int j = i + 1; j <= N; ++j) {
            current_water += W[j];
            if (current_water <= L[j]) {
                current_cost += P[j];
            }
        }
        
        if (min_cost == -1 || current_cost < min_cost) {
            min_cost = current_cost;
            best_start = i;
        }
    }

    vector<int> best_levels;
    if (best_start != -1) {
        int current_water = W[best_start];
        best_levels.push_back(best_start);
        for (int j = best_start + 1; j <= N; ++j) {
            current_water += W[j];
            if (current_water <= L[j]) {
                best_levels.push_back(j);
            }
        }
    }

    for (int i = 0; i < (int)best_levels.size(); ++i) {
        cout << best_levels[i] << "\n";
    }

    return 0;
}
