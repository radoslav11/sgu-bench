#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> L(N);
    for (int i = 0; i < N; i++) {
        cin >> L[i];
    }

    sort(L.begin(), L.end());

    int left = 0;
    int right = N - 1;
    int time = 0;

    while (left < right) {
        if (L[left] > 0) {
            L[left]--;
            right--;
            time++;
            if (L[left] == 0) {
                left++;
            }
        } else {
            left++;
        }
    }

    cout << time << "\n";
    return 0;
}
