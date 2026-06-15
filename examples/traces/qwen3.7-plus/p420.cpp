#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

long long fact[20];
long long pow10[20];

struct Signature {
    vector<int> counts;
    long long N;
};

vector<Signature> valid_sigs[17];

long long compute_N(const vector<int>& counts) {
    int L = 0;
    for (int c : counts) L += c;
    if (L == 0) return 0;
    long long num_permutations = fact[L];
    for (int c : counts) num_permutations /= fact[c];
    
    long long invalid = 0;
    if (counts[0] > 0) {
        invalid = fact[L - 1];
        invalid /= fact[counts[0] - 1];
        for (int i = 1; i <= 9; ++i) {
            invalid /= fact[counts[i]];
        }
    }
    return num_permutations - invalid;
}

void gen_compositions(int L, int pos, vector<int>& counts, vector<Signature>& sigs) {
    if (pos == 9) {
        counts[9] = L;
        long long N = compute_N(counts);
        if (N <= 200000) {
            sigs.push_back({counts, N});
        }
        return;
    }
    for (int i = 0; i <= L; ++i) {
        counts[pos] = i;
        gen_compositions(L - i, pos + 1, counts, sigs);
    }
}

long long get_sig(long long x) {
    long long sig = 0;
    int counts[10] = {0};
    while (x > 0) {
        counts[x % 10]++;
        x /= 10;
    }
    for (int i = 0; i < 10; ++i) {
        sig = (sig << 4) | counts[i];
    }
    return sig;
}

void count_valid(int pos, long long current_val, vector<int>& counts, long long A, long long B, int L, int& count) {
    if (count >= 3) return;
    if (pos == L) {
        if (current_val >= A && current_val <= B) {
            count++;
        }
        return;
    }
    for (int d = (pos == 0 ? 1 : 0); d <= 9; ++d) {
        if (counts[d] > 0) {
            counts[d]--;
            
            long long min_r = 0, max_r = 0;
            vector<int> c = counts;
            for (int digit = 1; digit <= 9; ++digit) {
                if (c[digit] > 0) { min_r = min_r * 10 + digit; c[digit]--; break; }
            }
            for (int digit = 0; digit <= 9; ++digit) {
                while (c[digit] > 0) { min_r = min_r * 10 + digit; c[digit]--; }
            }
            c = counts;
            for (int digit = 9; digit >= 0; --digit) {
                while (c[digit] > 0) { max_r = max_r * 10 + digit; c[digit]--; }
            }
            
            long long next_min = current_val * pow10[L - 1 - pos] + min_r;
            long long next_max = current_val * pow10[L - 1 - pos] + max_r;
            
            if (next_max < A || next_min > B) {
                counts[d]++;
                continue;
            }
            
            count_valid(pos + 1, current_val * 10 + d, counts, A, B, L, count);
            if (count >= 3) {
                counts[d]++;
                return;
            }
            counts[d]++;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    fact[0] = 1;
    pow10[0] = 1;
    for (int i = 1; i <= 16; ++i) {
        fact[i] = fact[i - 1] * i;
        pow10[i] = pow10[i - 1] * 10;
    }

    for (int L = 1; L <= 16; ++L) {
        vector<int> counts(10, 0);
        gen_compositions(L, 0, counts, valid_sigs[L]);
    }

    long long l, r;
    if (!(cin >> l >> r)) return 0;

    string sl = to_string(l);
    string sr = to_string(r);
    int len_l = sl.length();
    int len_r = sr.length();

    long long ans = 0;

    for (int L = len_l; L <= len_r; ++L) {
        long long A = (L == len_l) ? l : pow10[L - 1];
        long long B = (L == len_r) ? r : pow10[L] - 1;
        
        if (A > B) continue;

        if (B - A <= 10000000) {
            unordered_map<long long, int> freq;
            for (long long x = A; x <= B; ++x) {
                long long sig = get_sig(x);
                if (freq[sig] == 1) {
                    ans += 2;
                } else if (freq[sig] == 2) {
                    ans -= 2;
                }
                freq[sig]++;
            }
        } else {
            for (const auto& sig_struct : valid_sigs[L]) {
                if (sig_struct.N == 0) continue;
                int count = 0;
                vector<int> counts = sig_struct.counts;
                long long min_val = 0, max_val = 0;
                vector<int> c = counts;
                for (int d = 1; d <= 9; ++d) {
                    if (c[d] > 0) { min_val = min_val * 10 + d; c[d]--; break; }
                }
                for (int d = 0; d <= 9; ++d) {
                    while (c[d] > 0) { min_val = min_val * 10 + d; c[d]--; }
                }
                c = counts;
                for (int d = 9; d >= 0; --d<think>
The logic holds and correctly manages both small and large ranges efficiently. The DFS aggressively prunes paths that cannot possibly reach the target interval `[A, B]`, ensuring the solution runs well within the 0.75s time limit even for large intervals. The precomputation step is also extremely fast, bounded by the number of integer compositions.
</think>

