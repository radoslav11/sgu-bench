#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    long long sum = 0;
    for (int i = 0; i < M; ++i) {
        int S;
        cin >> S;
        sum += S;
    }

    cout << sum % N << '\n';

    return 0;
}
