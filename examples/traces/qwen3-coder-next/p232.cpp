#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int max_rot(string s) {
    int n = s.length();
    if (n == 0) return 0;
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        char a = s[(i + k) % n];
        char b = s[(j + k) % n];
        if (a == b) {
            k++;
        } else {
            if (a > b) {
                j += k + 1;
            } else {
                i += k + 1;
            }
            if (i == j) {
                j++;
            }
            k = 0;
        }
    }
    return min(i, j);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    long long N, K;
    cin >> N >> K;
    string D;
    cin >> D;

    long long g = gcd(N, K);
    long long T = N / g;

    vector<string> bases;
    vector<int> starts;

    for (long long r = 0; r < g; r++) {
        string s = "";
        long long current = r;
        for (long long j = 0; j < T; j++) {
            s += D[current];
            current = (current + K) % N;
        }
        int start = max_rot(s);
        bases.push_back(s);
        starts.push_back(start);
    }

    int best_cycle = 0;
    for (int r = 1; r < g; r++) {
        bool better = false;
        for (int offset = 0; offset < T; offset++) {
            int pos1 = (starts[best_cycle] + offset) % T;
            int pos2 = (starts[r] + offset) % T;
            if (bases[best_cycle][pos1] < bases[r][pos2]) {
                better = true;
                break;
            } else if (bases[best_cycle][pos1] > bases[r][pos2]) {
                break;
            }
        }
        if (better) {
            best_cycle = r;
        }
    }

    for (int i = 0; i < N; i++) {
        int pos = (starts[best_cycle] + i) % T;
        cout << bases[best_cycle][pos];
    }
    cout << '\n';

    return 0;
}
