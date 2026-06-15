#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<vector<int>> departures(M + 1);
    vector<int> max_dep(M + 1, 0);

    for (int i = 0; i < N; ++i) {
        int k, t;
        cin >> k >> t;

        int best_j = 1;
        int min_people = 1e9;

        for (int j = 1; j <= M; ++j) {
            int count = 0;
            for (int dep : departures[j]) {
                if (dep > k) {
                    count++;
                }
            }
            if (count < min_people) {
                min_people = count;
                best_j = j;
            }
        }

        int dep_time = max(k, max_dep[best_j]) + t;
        departures[best_j].push_back(dep_time);
        max_dep[best_j] = dep_time;

        cout << best_j << " " << dep_time << "\n";
    }

    return 0;
}
