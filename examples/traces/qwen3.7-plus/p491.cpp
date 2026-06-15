#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    if (N < 3) {
        cout << 0 << "\n";
        return 0;
    }

    vector<int> num_divs(N, 0);
    for (int i = 1; i < N; ++i) {
        for (int j = i; j < N; j += i) {
            num_divs[j]++;
        }
    }

    vector<int> divs_start(N + 1, 0);
    for (int i = 1; i < N; ++i) {
        divs_start[i + 1] = divs_start[i] + num_divs[i];
    }

    int total_divs = divs_start[N];
    vector<int> all_divs(total_divs);
    vector<int> current_pos = divs_start;

    for (int i = 1; i < N; ++i) {
        for (int j = i; j < N; j += i) {
            all_divs[current_pos[j]++] = i;
        }
    }

    long long ans = 0;
    vector<int> seen(N, 0);

    for (int B = 2; B < N; ++B) {
        int start = N % B;
        if (start == 0) start = B;
        for (int R = start; R < N; R += B) {
            int start_idx = divs_start[R];
            int end_idx = divs_start[R + 1];
            for (int idx = start_idx; idx < end_idx; ++idx) {
                int A = all_divs[idx];
                if (A >= B) break;
                if (seen[A] != B) {
                    seen[A] = B;
                    ans++;
                }
            }
        }
    }

    cout << ans << "\n";
    return 0;
}
