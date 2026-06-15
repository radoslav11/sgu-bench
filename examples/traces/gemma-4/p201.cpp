#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

using namespace std;

/**
 * Problem: 201. Non Absorbing DFA
 * This problem asks us to count strings of length N accepted by a DFA that includes non-absorbing edges.
 * A non-absorbing edge for a character c means the character is not consumed, and the DFA moves to a new state.
 * We pre-calculate the effective transition for each state and each character, including the absorption
 * of a character after potential non-absorbing steps.
 * Because the result can be as large as 26^60, we use a custom BigInt class.
 */

const long long BASE = 1000000000;

struct BigInt {
    long long digits[12];
    int len;

    BigInt(long long v = 0) {
        memset(digits, 0, sizeof(digits));
        if (v == 0) {
            len = 1;
        } else {
            len = 0;
            while (v > 0) {
                digits[len++] = v % BASE;
                v /= BASE;
            }
        }
    }

    // Addition function to add another BigInt into this one.
    void add(const BigInt& other) {
        int max_len = max(len, other.len);
        long long carry = 0;
        for (int i = 0; i < max_len || carry; ++i) {
            long long sum = (i < len ? digits[i] : 0) + 
                            (i < other.len ? other.digits[i] : 0) + 
                            carry;
            digits[i] = sum % BASE;
            carry = sum / BASE;
            if (i + 1 > len) len = i + 1;
        }
        while (len > 1 && digits[len - 1] == 0) len--;
    }

    // Function to print the BigInt in standard base-10 format.
    void print() const {
        if (len == 0) {
            printf("0\n");
            return;
        }
        printf("%lld", digits[len - 1]);
        for (int i = len - 2; i >= 0; --i) {
            printf("%09lld", digits[i]);
        }
        printf("\n");
    }
};

int phi[1001][26];
int chi[1001][26];
int f[1001][26];
bool visited[1001];
BigInt dp[61][1001];

int main() {
    char sigma_buf[100];
    // Read the input alphabet.
    if (scanf("%s", sigma_buf) != 1) return 0;
    string sigma_str = sigma_buf;
    int sigma_len = sigma_str.length();

    // Read K, S, L, and the set of terminal states T.
    int K, S, L;
    if (scanf("%d %d %d", &K, &S, &L) != 3) return 0;
    vector<int> T(L);
    for (int i = 0; i < L; ++i) scanf("%d", &T[i]);

    // Read transition function phi.
    for (int i = 1; i <= K; ++i) {
        for (int j = 0; j < sigma_len; ++j) {
            scanf("%d", &phi[i][j]);
        }
    }
    // Read absorbing/non-absorbing function chi.
    for (int i = 1; i <= K; ++i) {
        for (int j = 0; j < sigma_len; ++j) {
            scanf("%d", &chi[i][j]);
        }
    }

    // Read target string length N.
    int N;
    if (scanf("%d", &N) != 1) return 0;

    // Pre-calculate the destination state after consuming each character for each starting state.
    // This accounts for any potential non-absorbing transitions.
    for (int c = 0; c < sigma_len; ++c) {
        for (int u = 1; u <= K; ++u) {
            int curr = u;
            memset(visited, 0, sizeof(visited));
            bool loop = false;
            // Traverse non-absorbing transitions for character c.
            while (chi[curr][c] == 1) {
                if (visited[curr]) {
                    loop = true;
                    break;
                }
                visited[curr] = true;
                curr = phi[curr][c];
            }
            // If we hit a loop, character c can never be consumed from this state.
            if (loop) {
                f[u][c] = -1;
            } else {
                // The final absorbing transition consumes the character.
                f[u][c] = phi[curr][c];
            }
        }
    }

    // DP table initialization: dp[i][u] = number of strings of length i that land the DFA in state u.
    dp[0][S] = BigInt(1);
    for (int i = 0; i < N; ++i) {
        for (int u = 1; u <= K; ++u) {
            // Skip if no strings of length i land in state u.
            if (dp[i][u].len == 1 && dp[i][u].digits[0] == 0) continue;
            for (int c = 0; c < sigma_len; ++c) {
                int v = f[u][c];
                if (v != -1) {
                    dp[i + 1][v].add(dp[i][u]);
                }
            }
        }
    }

    // Sum up the counts for all terminal states after N characters are consumed.
    BigInt ans(0);
    for (int t : T) {
        ans.add(dp[N][t]);
    }
    ans.print();

    return 0;
}

