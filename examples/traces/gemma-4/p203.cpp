#include <cstdio>

using namespace std;

/**
 * Problem Summary:
 * We need to find the total length of an encoded text given the frequencies of its characters.
 * Huffman encoding works by repeatedly merging the two smallest active nodes. 
 * The length of the encoded text in bits is equal to the sum of all internal node values 
 * generated during the Huffman tree construction.
 * 
 * Constraints:
 * N <= 500,000
 * P_i <= 10^9
 * P_i <= P_{i+1} (frequencies are given in non-decreasing order)
 * Time limit: 0.25s (very tight!)
 * 
 * Complexity:
 * O(N) time using a two-queue approach (one queue for initial frequencies, one for merged sums).
 * O(N) space to store the frequencies and merged sums.
 */

typedef __int128_t int128;

// Arrays to store frequencies and merged sums.
// Using arrays instead of std::queue for maximum performance under the tight time limit.
long long P[500005];
long long Q2[500005];
int q1_idx = 0, q1_end = 0;
int q2_idx = 0, q2_end = 0;

// Fast I/O setup using fread
char input_buf[1 << 20];
int input_pos = 0, input_len = 0;

inline char get_char() {
    if (input_pos == input_len) {
        input_pos = 0;
        input_len = fread(input_buf, 1, 1 << 20, stdin);
        if (input_len == 0) return EOF;
    }
    return input_buf[input_pos++];
}

inline int readInt() {
    int x = 0;
    char c = get_char();
    while (c != EOF && (c < '0' || c > '9')) c = get_char();
    if (c == EOF) return -1;
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = get_char();
    }
    return x;
}

inline long long readLL() {
    long long x = 0;
    char c = get_char();
    while (c != EOF && (c < '0' || c > '9')) c = get_char();
    if (c == EOF) return -1;
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = get_char();
    }
    return x;
}

/**
 * Function to get the smallest available value from either the original frequencies
 * (Q1) or the merged sums (Q2). Since P_i is non-decreasing and sums are added in
 * non-decreasing order, both queues are always sorted.
 */
inline long long get_min() {
    if (q1_idx == q1_end) return Q2[q2_idx++];
    if (q2_idx == q2_end) return P[q1_idx++];
    if (P[q1_idx] < Q2[q2_idx]) return P[q1_idx++];
    else return Q2[q2_idx++];
}

/**
 * Custom print function for __int128, as standard printf doesn't support it.
 */
void print_int128(int128 n) {
    if (n == 0) {
        putchar('0');
        putchar('\n');
        return;
    }
    char out_buf[64];
    int out_pos = 0;
    while (n > 0) {
        out_buf[out_pos++] = (char)(n % 10 + '0');
        n /= 10;
    }
    for (int i = out_pos - 1; i >= 0; --i) {
        putchar(out_buf[i]);
    }
    putchar('\n');
}

int main() {
    int N = readInt();
    if (N <= 0) return 0;

    // Read frequencies
    for (int i = 0; i < N; ++i) {
        P[i] = readLL();
    }
    q1_end = N;

    // Huffman encoding using two queues
    int128 total_bits = 0;
    for (int i = 0; i < N - 1; ++i) {
        long long x = get_min();
        long long y = get_min();
        long long sum = x + y;
        // The total bit length is the sum of all internal node values.
        // x + y can be up to ~10^15, so it fits in long long.
        total_bits += (int128)sum;
        Q2[q2_end++] = sum;
    }

    // Output the total length in bits
    print_int128(total_bits);

    return 0;
}

