#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int main() {
    int N;
    cin >> N;
    
    // The minimal number of colors needed is the maximum number of 
    // times any prime factor appears in the factorization of numbers 1..N.
    // Actually, this is equivalent to the maximum number of integers in 
    // a chain where each divides the next: 1, 2, 4, 8, ... or 1, 3, 6, 12, ...
    // But more precisely, the minimal number of colors is the maximum exponent 
    // in the prime factorization of any number in [1, N], plus 1? 
    // Actually, consider: for each number, we can assign color based on the number 
    // of times 2 divides it (i.e., the exponent of 2 in its factorization) + 1.
    // However, the constraint is: if A divisible by B, then color[A] != color[B].
    // So if we assign color based on the highest power of 2 dividing the number, 
    // then for any chain like x, 2x, 4x, 8x, ... the colors would be distinct.
    // But what about numbers with different prime factors?
    // Actually, the minimal number of colors is the size of the largest antichain 
    // in the divisibility poset, which by Dilworth's theorem is the minimum number 
    // of chains needed to cover the set. But here we want the chromatic number of 
    // the comparability graph? Actually, the graph is: edge between A and B if A|B or B|A.
    // This graph is a comparability graph of a poset, and the chromatic number equals 
    // the size of the largest clique. A clique in this graph is a set of numbers where 
    // every pair is comparable (i.e., a chain). So the chromatic number is the size of 
    // the largest chain. 
    // The largest chain is the longest sequence: a1 | a2 | ... | ak with ak <= N.
    // To maximize k, we take the smallest starting number (1) and multiply by 2 each time: 
    // 1, 2, 4, 8, 16, ... so k = floor(log2(N)) + 1.
    // But wait: consider N=12: 
    //   chain: 1,2,4,8 -> length 4; 1,3,6,12 -> length 4; 1,2,6,12 -> length 4.
    // So the maximum chain length is 4, and the example output uses 4 colors.
    // Therefore, the minimal number of colors M is the length of the longest chain.
    
    // How to compute M?
    // M = max_{x in [1, N]} (number of divisors in a chain starting from x)
    // Actually, we can compute the maximum chain length ending at each number.
    // Let dp[i] = length of the longest chain ending at i (with i as the largest element).
    // Then dp[i] = 1 + max{ dp[j] for j | i and j < i }.
    // Then M = max(dp[1..N]).
    
    // However, note: N can be up to? The problem doesn't specify, but time limit 0.25s and memory 64KB.
    // The input constraint is missing, but typically N <= 10^5 or 10^6? 
    // But 0.25s and 64KB suggests N is not too big. However, the sample N=12.
    // Let's assume N up to 10^5 or 10^6. We need an efficient way.
    
    // Alternative: the longest chain is the maximum over k such that 2^(k-1) <= N, so k = floor(log2(N)) + 1?
    // But for N=12: log2(12) ~ 3.58, so k=4 -> correct.
    // However, consider N=6: 
    //   chains: 1,2,4 -> length 3; 1,2,6 -> length 3; 1,3,6 -> length 3.
    //   log2(6) ~ 2.58 -> k=3 -> correct.
    // But what about using base 3? 1,3,6 -> same length. 
    // Actually, the longest chain is achieved by multiplying by the smallest prime (2) each time, 
    // so the maximum chain length is floor(log2(N)) + 1.
    // However, consider N=9: 
    //   chain: 1,2,4,8 -> length 4? but 8<=9 -> yes. 
    //   log2(9)=3.17 -> k=4 -> correct.
    // But what about 1,3,9? -> length 3, which is less than 4.
    // So indeed, the longest chain is the maximum k such that 2^(k-1) <= N -> k = floor(log2(N)) + 1.
    // However, wait: what if we use a different base? 
    //   For N=16: 
    //      base 2: 1,2,4,8,16 -> length 5.
    //      base 3: 1,3,9 -> length 3.
    //      base 5: 1,5 -> length 2.
    // So base 2 gives the longest.
    // Therefore, M = floor(log2(N)) + 1? 
    // But consider N=1: M=1 -> log2(1)=0 -> 0+1=1 -> correct.
    // N=2: log2(2)=1 -> 1+1=2 -> correct (chain: 1,2).
    // However, let me check with the example: N=12 -> floor(log2(12)) = 3 -> 3+1=4 -> correct.
    
    // But wait: is it always floor(log2(N)) + 1? 
    // What about N=3: 
    //   chains: 1,2 -> length 2; 1,3 -> length 2; so M=2.
    //   floor(log2(3)) = 1 -> 1+1=2 -> correct.
    // N=5: 
    //   chain: 1,2,4 -> length 3 -> M=3.
    //   floor(log2(5))=2 -> 2+1=3 -> correct.
    
    // Therefore, M = number of bits in the binary representation of N? 
    // Actually, the maximum chain length is the maximum k such that there exists a chain 
    // x1 | x2 | ... | xk with xk <= N.
    // The minimal xk for a chain of length k is 2^(k-1) (by taking x1=1, x2=2, x3=4, ...).
    // So k is the largest integer such that 2^(k-1) <= N -> k = floor(log2(N)) + 1.
    
    // So we can compute M as:
    int M = 0;
    int temp = N;
    while (temp) {
        M++;
        temp /= 2;
    }
    // But note: for N=1: 
    //   temp=1 -> M=1, then temp=0 -> M=1 -> correct.
    // However, the above loop counts the number of times we can divide by 2 until 0, which is floor(log2(N))+1.
    // Alternatively, we can do:
    //   M = 0;
    //   for (int x = 1; x <= N; x *= 2) {
    //       M++;
    //   }
    // But note: x starts at 1, then 2, 4, ... until <= N.
    
    // However, let's verify with N=1: 
    //   x=1 -> M=1, then x=2 -> 2>1 -> break -> M=1 -> correct.
    // N=2: x=1 -> M=1; x=2 -> M=2; x=4 -> break -> M=2 -> correct.
    // So we can do:
    M = 0;
    for (int x = 1; x <= N; x *= 2) {
        M++;
    }
    
    // Now, how to assign colors? 
    // We can assign color[i] = the exponent of 2 in the factorization of i, plus 1.
    // Why? 
    //   If A is divisible by B, then the exponent of 2 in B is <= exponent of 2 in A.
    //   But if the exponent of 2 in A equals that in B, then we might have A divisible by B? 
    //   Example: A=6, B=3: 6 is divisible by 3, but exponent of 2 in 6 is 1, in 3 is 0 -> different.
    //   However, what about A=12, B=6: 12 divisible by 6 -> exponent of 2 in 12 is 2, in 6 is 1 -> different.
    //   But what if A=9, B=3: 9 divisible by 3 -> exponent of 2 in 9 is 0, in 3 is 0 -> same! -> conflict.
    // So that doesn't work for odd numbers.
    
    // Alternative: assign color[i] = the number of times we can divide i by primes until we get an odd number? 
    // Actually, the problem is: we need that if A is divisible by B, then color[A] != color[B].
    // Notice that if A is divisible by B, then in the chain of divisors, the "level" (like the length of the chain from 1 to the number) must be different.
    // So let level[i] = length of the longest chain from 1 to i (i.e., the maximum k such that there is a chain 1 = a1 | a2 | ... | ak = i).
    // Then level[i] = 1 + max{ level[j] for j | i, j < i }.
    // And then we can assign color[i] = level[i].
    // Why? If A is divisible by B, then level[A] >= level[B] + (at least 1) because we can extend the chain for B by multiplying to get to A? 
    // Actually, not necessarily: if A is divisible by B, then we can form a chain for A by taking the chain for B and then multiplying by (A/B). 
    // So level[A] >= level[B] + 1? Not exactly: the chain for A might not go through B. But we know that the chain for A must be at least level[B] + 1 because we can go 1 -> ... -> B -> A.
    // Therefore, level[A] >= level[B] + 1, so level[A] > level[B]. Hence, color[A] != color[B].
    // So this assignment works.
    // And the number of colors used is max(level[1..N]) = M (the chain length we computed).
    
    // How to compute level[i] for i from 1 to N?
    // We can do:
    //   level[1] = 1.
    //   for i from 2 to N:
    //        level[i] = 1;
    //        for each divisor j of i (j < i), level[i] = max(level[i], level[j] + 1);
    // But iterating over all divisors for each i is O(N * sqrt(N)) which might be too slow for N up to 10^5? 
    // Actually, we can do better: 
    //   Instead, for each j from 1 to N, we can update all multiples of j: 
    //        level[multiple] = max(level[multiple], level[j] + 1);
    //   This is O(N log N) because for each j, we do N/j steps -> harmonic series.
    
    // Steps:
    //   vector<int> level(N+1, 1);
    //   for (int j = 1; j <= N; j++) {
    //       for (int k = 2*j; k <= N; k += j) {
    //           level[k] = max(level[k], level[j] + 1);
    //       }
    //   }
    //   Then M = *max_element(level.begin()+1, level.end());
    //   But note: we already computed M by the power of 2 method, so we know M. 
    //   However, the problem requires the minimal number of colors, which is M (the max chain length). 
    //   And the level assignment uses exactly M colors (from 1 to M).
    
    // But note: the problem says "minimal number of colors", and we know M is minimal (the chromatic number).
    // So we can compute the level array and then output the levels.
    
    // However, the constraints: N up to? 
    //   The problem says 0.25s and 64KB. 
    //   The double loop: for j=1 to N, inner loop N/j -> total ~ N * (1 + 1/2 + 1/3 + ... + 1/N) ~ N * ln N.
    //   For N=10^5, ln N ~ 12, so 1.2e6 iterations -> acceptable in 0.25s.
    //   For N=10^6, ln N ~ 14, so 14e6 iterations -> acceptable in C++ in 0.25s.
    //   But the problem doesn't specify the upper bound of N. However, the memory is 64KB: 
    //        We need an array of N integers -> for N=10^6, 4MB -> too much? 
    //        64KB = 65536 bytes -> can hold about 16000 integers (4 bytes each) -> so N <= 16000?
    //   Actually, the problem statement says "N ()", so the constraint is missing. 
    //   But the sample N=12, and the problem is from a contest, so likely N is up to 10^4 or 10^5.
    //   However, 64KB memory limit: 
    //        We need to store an array of N integers: 4*N bytes. 
    //        4*N <= 65536 -> N <= 16384.
    //   So N is at most 16384? 
    //   Therefore, we can do O(N log N) without problem.
    
    // Let's do:
    //   Step 1: Read N.
    //   Step 2: Compute the level array for 1..N.
    //   Step 3: Find M = max(level[1..N]).
    //   Step 4: Output M.
    //   Step 5: Output level[1], level[2], ... level[N].

    vector<int> level(N+1, 1);
    // level[1] = 1 (already set by initialization)
    for (int j = 1; j <= N; j++) {
        for (int k = 2*j; k <= N; k += j) {
            if (level[k] < level[j] + 1) {
                level[k] = level[j] + 1;
            }
        }
    }
    
    int maxColor = 0;
    for (int i = 1; i <= N; i++) {
        if (level[i] > maxColor) {
            maxColor = level[i];
        }
    }
    
    cout << maxColor << endl;
    for (int i = 1; i <= N; i++) {
        if (i > 1) cout << " ";
        cout << level[i];
    }
    cout << endl;
    
    return 0;
}
