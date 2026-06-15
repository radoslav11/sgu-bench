#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Using 2^61 - 1 as a large prime for rolling hash to minimize collisions.
typedef unsigned long long ull;
typedef __int128_t u128;

const ull MOD = (1ULL << 61) - 1;

// Modular multiplication for 2^61 - 1.
ull mul(ull a, ull b) {
    u128 res = (u128)a * b;
    ull ans = (ull)(res >> 61) + (ull)(res & MOD);
    if (ans >= MOD) ans -= MOD;
    return ans;
}

ull add(ull a, ull b) {
    ull ans = a + b;
    if (ans >= MOD) ans -= MOD;
    return ans;
}

ull sub(ull a, ull b) {
    ull ans = a + MOD - b;
    if (ans >= MOD) ans -= MOD;
    return ans;
}

// Booth's algorithm or similar to find the lexicographically smallest cyclic shift in O(M).
string get_smallest_cyclic_shift(string s) {
    int n = s.length();
    string ss = s + s;
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        if (ss[i + k] == ss[j + k]) {
            k++;
        } else {
            if (ss[i + k] > ss[j + k]) i += k + 1;
            else j += k + 1;
            if (i == j) j++;
            k = 0;
        }
    }
    int start = min(i, j);
    return ss.substr(start, n);
}

// Rolling hash implementation.
struct HashString {
    vector<ull> h;
    vector<ull> p;
    ull base;

    HashString(const string& s, ull b) : base(b) {
        int n = s.length();
        h.assign(n + 1, 0);
        p.assign(n + 1, 1);
        for (int i = 0; i < n; i++) {
            h[i + 1] = add(mul(h[i], base), (ull)s[i]);
            p[i + 1] = mul(p[i], base);
        }
    }

    ull get_hash(int l, int r) {
        return sub(h[r + 1], mul(h[l], p[r - l + 1]));
    }
};

struct Candidate {
    int cost;
    int dir; // 0 for LEFT, 1 for RIGHT
    int i;

    bool operator<(const Candidate& other) const {
        if (cost != other.cost) return cost < other.cost;
        return dir < other.dir;
    }
};

int main() {
    // Fast I/O.
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) return 0;
    string S, T;
    cin >> S >> T;

    // Step 1: Find the smallest cyclic shift of the second tape.
    string T_min = get_smallest_cyclic_shift(T);

    // Step 2: Prepare hashing for S and T_min. 
    // S_ext handles cyclic shifts of S by using a larger string S+S.
    string S_ext = S + S;
    if ((int)S_ext.length() < N + M) {
        S_ext += S.substr(0, N + M - S_ext.length());
    }

    ull base = 137; // Prime base for hash.
    HashString hs_S(S_ext, base);
    HashString hs_T(T_min, base);

    vector<Candidate> candidates;
    // Step 3: Check all cyclic shifts of S for at most one mismatch.
    for (int i = 0; i < N; i++) {
        int mismatches = 0;
        int curr = 0;
        while (curr < M) {
            // Find the first mismatch in the current range using binary search + hashing.
            int low = curr, high = M - 1, first_mismatch = -1;
            while (low <= high) {
                int mid = low + (high - low) / 2;
                if (hs_S.get_hash(i + curr, i + mid) == hs_T.get_hash(curr, mid)) {
                    low = mid + 1;
                } else {
                    first_mismatch = mid;
                    high = mid - 1;
                }
            }

            if (first_mismatch == -1) break; // No mismatch in the remaining part.
            mismatches++;
            if (mismatches > 1) break;
            curr = first_mismatch + 1;
        }

        if (mismatches <= 1) {
            // If valid, add both possible directions (left and right shifts) as candidates.
            // Left shift of i positions starts at S[i].
            // Right shift of k positions starts at S[N-k].
            // If i=0, only one candidate (cost 0).
            if (i == 0) {
                candidates.push_back({0, 0, 0});
            } else {
                candidates.push_back({i, 0, i});           // i left shifts
                candidates.push_back({N - i, 1, i});       // N-i right shifts
            }
        }
    }

    // Step 4: Sort candidates by minimal cost, then tie-break with left shift.
    sort(candidates.begin(), candidates.end());

    // The result is derived from the best candidate.
    if (!candidates.empty()) {
        Candidate best = candidates[0];
        cout << T_min << "\n";
        // A cyclic shift starting at index i is represented by S_ext[i...i+N-1].
        cout << S_ext.substr(best.i, N) << "\n";
    }

    return 0;
}

