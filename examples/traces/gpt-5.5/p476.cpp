#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

int N, K;
vector<unsigned long long> tripleMask;
vector<unsigned long long> cnt;

void dfs(int idx, unsigned long long used, int chosen) {
    if (idx == K) {
        cnt[chosen]++;
        return;
    }

    dfs(idx + 1, used, chosen);

    if ((used & tripleMask[idx]) == 0) {
        dfs(idx + 1, used | tripleMask[idx], chosen + 1);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> K;

    map<int, int> id;
    tripleMask.resize(K);

    for (int i = 0; i < K; ++i) {
        int a, b, c;
        cin >> a >> b >> c;

        if (!id.count(a)) id[a] = id.size();
        if (!id.count(b)) id[b] = id.size();
        if (!id.count(c)) id[c] = id.size();

        tripleMask[i] = (1ULL << id[a]) | (1ULL << id[b]) | (1ULL << id[c]);
    }

    vector<cpp_int> ways(N + 1);
    ways[0] = 1;

    for (int i = 1; i <= N; ++i) {
        ways[i] = ways[i - 1] * (3 * i - 1) * (3 * i - 2) / 2;
    }

    cnt.assign(K + 1, 0);
    dfs(0, 0, 0);

    cpp_int answer = 0;

    for (int i = 0; i <= K && i <= N; ++i) {
        cpp_int term = ways[N - i] * cnt[i];

        if (i % 2 == 0) {
            answer += term;
        } else {
            answer -= term;
        }
    }

    cout << answer << '\n';

    return 0;
}
