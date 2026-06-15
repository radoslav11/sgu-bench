#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    int A0, B0;
    cin >> N >> A0 >> B0;

    vector<pair<int, int>> v;
    v.reserve(N);
    for(int k = 0; k < N; ++k) {
        int A = (int)((1LL * k * A0) % N);
        int B = (int)((1LL * k * B0) % N);
        v.emplace_back(A, B);
    }

    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());

    cout << v.size() << "\n";
    for(auto& p: v) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}
