#include <iostream>
#include <vector>
using namespace std;

int main() {
    int M, N;
    cin >> M >> N;

    vector<pair<int, int>> volunteers;
    for (int i = 0; i < N; i++) {
        int A, B;
        cin >> A >> B;
        volunteers.push_back(make_pair(A, B));
    }

    // Binary search for minimal time T
    int low = 0;
    int high = 10000; // Upper bound: worst-case M=100, worst volunteer: A=1, B=4 -> 5 minutes per balloon -> 500 minutes, so 10000 is safe.

    while (low < high) {
        int T = (low + high) / 2;
        long long totalBalloons = 0;

        for (int i = 0; i < N; i++) {
            int A = volunteers[i].first;
            int B = volunteers[i].second;

            if (T > 0) {
                // Number of minutes volunteer i can work in T minutes (minutes 0 to T-1)
                int workMinutes = (T - 1) / (B + 1) + 1;
                totalBalloons += static_cast<long long>(workMinutes) * A;
            }
        }

        if (totalBalloons >= M) {
            high = T;
        } else {
            low = T + 1;
        }
    }

    cout << low << endl;

    return 0;
}
