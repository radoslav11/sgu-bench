#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

int main() {
    long long N, A0, B0;
    cin >> N >> A0 >> B0;

    long long g = gcd(N, A0);
    g = gcd(g, B0);
    long long t = N / g;

    vector<pair<long long, long long>> pairs;
    for (long long k = 0; k < t; k++) {
        long long A = (k * A0) % N;
        long long B = (k * B0) % N;
        pairs.push_back({A, B});
    }

    sort(pairs.begin(), pairs.end());

    cout << t << '\n';
    for (const auto& p : pairs) {
        cout << p.first << " " << p.second << '\n';
    }

    return 0;
}
