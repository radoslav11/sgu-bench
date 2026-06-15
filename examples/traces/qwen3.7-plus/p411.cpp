#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

struct Hash {
    uint64_t h1, h2;
    bool operator==(const Hash& o) const { return h1 == o.h1 && h2 == o.h2; }
    bool operator<(const Hash& o) const { 
        if (h1 != o.h1) return h1 < o.h1;
        return h2 < o.h2;
    }
};

vector<Hash> pref1, pref2;
vector<uint64_t> p1_1, p1_2;
vector<uint64_t> p2_1, p2_2;

void build_hash(const string& S, vector<Hash>& pref, vector<uint64_t>& p1, vector<uint64_t>& p2, uint64_t b1, uint64_t b2) {
    int n = S.length();
    pref.resize(n + 1, {0, 0});
    p1.resize(n + 1, 1);
    p2.resize(n + 1, 1);
    for (int i = 0; i < n; ++i) {
        p1[i + 1] = p1[i] * b1;
        p2[i + 1] = p2[i] * b2;
        pref[i + 1].h1 = pref[i].h1 * b1 + S[i];
        pref[i + 1].h2 = pref[i].h2 * b2 + S[i];
    }
}

Hash get_hash(const vector<Hash>& pref, const vector<uint64_t>& p1, const vector<uint64_t>& p2, int L, int R, uint64_t b1, uint64_t b2) {
    Hash res;
    res.h1 = pref[R + 1].h1 - pref[L].h1 * p1[R - L + 1];
    res.h2 = pref[R + 1].h2 - pref[L].h2 * p2[R - L + 1];
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string S1, S2;
    if (!(cin >> S1 >> S2)) return 0;

    int N = S1.length();
    int M = S2.length();

    mt19937_64 rng(1337);
    uint64_t b1 = rng() % 1000000000 + 1000000;
    uint64_t b2 = rng() % 1000000000 + 1000000;
    if (b1 % 2 == 0) b1++;
    if (b2 % 2 == 0) b2++;

    build_hash(S1, pref1, p1_1, p1_2, b1, b2);
    build_hash(S2, pref2, p2_1, p2_2, b1, b2);

    vector<Hash> S2_hashes;
    S2_hashes.reserve(M * (M + 1) / 2);

    for (int c = 0; c < 2 * M - 1; ++c) {
        if (c % 2 == 0) {
            int max_r = min(c / 2, M - 1 - c / 2);
            for (int r = 0; r <= max_r; ++r) {
                int L = c / 2 - r;
                int R = c / 2 + r;
                S2_hashes.push_back(get_hash(pref2, p2_1, p2_2, L, R, b1, b2));
            }
        } else {
            int left_idx = (c - 1) / 2;
            int right_idx = (c + 1) / 2;
            int max_r = min(left_idx + 1, M - right_idx);
            for (int r = 1; r <= max_r; ++r) {
                int L = left_idx - (r - 1);
                int R = right_idx + (r - 1);
                S2_hashes.push_back(get_hash(pref2, p2_1, p2_2, L, R, b1, b2));
            }
        }
    }

    sort(S2_hashes.begin(), S2_hashes.end());
    S2_hashes.erase(unique(S2_hashes.begin(), S2_hashes.end()), S2_hashes.end());
    S2_hashes.shrink_to_fit();

    vector<int> max_r_arr(2 * N - 1, 0);
    for (int c = 0; c < 2 * N - 1; ++c) {
        if (c % 2 == 0) {
            int max_possible = min(c / 2, N - 1 - c / 2);
            int r = 0;
            while (r <= max_possible && S1[c / 2 - r] == S1[c / 2 + r]) {
                r++;
            }
            max_r_arr[c] = r - 1;
        } else {
            int left_idx = (c - 1) / 2;
            int right_idx = (c + 1) / 2;
            int max_possible = min(left_idx + 1, N - right_idx);
            int r = 1;
            while (r <= max_possible && S1[left_idx - (r - 1)] == S1[right_idx + (r - 1)]) {
                r++;
            }
            max_r_arr[c] = r - 1;
        }
    }

    int best_len = 0;
    int best_L = 0;

    for (int c = 0; c < 2 * N - 1; ++c) {
        if (c % 2 == 0) {
            int low = 0, high = max_r_arr[c];
            int best_r = -1;
            while (low <= high) {
                int mid = low + (high - low) / 2;
                int L = c / 2 - mid;
                int R = c / 2 + mid;
                Hash h = get_hash(pref1, p1_1, p1_2, L, R, b1, b2);
                if (binary_search(S2_hashes.begin(), S2_hashes.end(), h)) {
                    best_r = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
            if (best_r != -1) {
                int curr_len = 2 * best_r + 1;
                if (curr_len > best_len) {
                    best_len = curr_len;
                    best_L = c / 2 - best_r;
                }
            }
        } else {
            int low = 1, high = max_r_arr[c];
            int best_r = 0;
            while (low <= high) {
                int mid = low + (high - low) / 2;
                int L = (c - 1) / 2 - (mid - 1);
                int R = (c + 1) / 2 + (mid - 1);
                Hash h = get_hash(pref1, p1_1, p1_2, L, R, b1, b2);
                if (binary_search(S2_hashes.begin(), S2_hashes.end(), h)) {
                    best_r = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
            if (best_r >= 1) {
                int curr_len = 2 * best_r;
                if (curr_len > best_len) {
                    best_len = curr_len;
                    best_L = (c - 1) / 2 - (best_r - 1);
                }
            }
        }
    }

    cout << S1.substr(best_L, best_len) << "\n";

    return 0;
}
