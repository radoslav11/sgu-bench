#include <iostream>

/**
 * Problem Analysis:
 * The sequence is defined as S_n = concatenation of integers 1, 2, ..., n.
 * A number is divisible by 3 if and only if the sum of its digits is divisible by 3.
 * Let D(i) be the sum of the digits of integer i.
 * The sum of digits of S_n is the sum of the sums of digits of each integer from 1 to n:
 * SumDigits(S_n) = D(1) + D(2) + ... + D(n).
 * 
 * Since D(i) ≡ i (mod 3), we have:
 * SumDigits(S_n) ≡ 1 + 2 + ... + n (mod 3)
 * SumDigits(S_n) ≡ n(n + 1) / 2 (mod 3).
 * 
 * S_n is divisible by 3 if SumDigits(S_n) ≡ 0 (mod 3).
 * This happens when n(n + 1) / 2 ≡ 0 (mod 3).
 * Since 2 is invertible modulo 3 (2 * 2 ≡ 1 mod 3),
 * this is equivalent to n(n + 1) ≡ 0 (mod 3).
 * 
 * n(n + 1) is divisible by 3 if either n ≡ 0 (mod 3) or n + 1 ≡ 0 (mod 3) (which means n ≡ 2 mod 3).
 * 
 * For n = 1, 2, 3, 4, 5, 6, ..., the sequence of n mod 3 is 1, 2, 0, 1, 2, 0, ...
 * The corresponding divisibility by 3 (using n ≡ 0 or 2 mod 3) is:
 * n=1: 1 mod 3 = 1 -> No
 * n=2: 2 mod 3 = 2 -> Yes
 * n=3: 3 mod 3 = 0 -> Yes
 * n=4: 4 mod 3 = 1 -> No
 * n=5: 5 mod 3 = 2 -> Yes
 * n=6: 6 mod 3 = 0 -> Yes
 * ...
 * 
 * The pattern of divisibility is (No, Yes, Yes) repeating every 3 elements.
 * The number of elements NOT divisible by 3 in the range [1, N] is the count of n such that n ≡ 1 (mod 3).
 * These are n = 1, 4, 7, ..., which is the sequence 3k + 1 for k = 0, 1, 2, ...
 * The number of such elements in [1, N] is floor((N - 1) / 3) + 1, which simplifies to floor((N + 2) / 3).
 * 
 * Therefore, the number of elements divisible by 3 in [1, N] is N - floor((N + 2) / 3).
 * 
 * Constraints:
 * N <= 2^31 - 1. Using long long is necessary to avoid overflow during calculations like N + 2.
 */

using namespace std;

int main() {
    // Speed up input and output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long N;
    if (!(cin >> N)) return 0;

    // N - floor((N + 2) / 3)
    // Using long long to handle N up to 2^31 - 1 and prevent overflow on N + 2.
    long long divisible_count = N - (N + 2) / 3;

    cout << divisible_count << endl;

    return 0;
}

