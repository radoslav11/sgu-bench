#include <iostream>

/**
 * Problem analysis:
 * Peter has a chain of N links and wants to pay one link each day for N days.
 * He can pay with a part of the chain and get back the remaining links in that part as change.
 * However, to get change in the form of individual links, those links must have been
 * already separated by broken links (gaps).
 * 
 * If Peter breaks B links, he creates B gaps. A single broken link can be considered 
 * a part of size 1. By breaking B links such that each broken link is a separate 
 * part, he gets B parts of size 1 from the broken links themselves, and the remaining 
 * N-B links are split into B+1 parts.
 * 
 * To maximize the number of days he can cover with B broken links, he should create 
 * as many 1-link parts as possible to use as change.
 * The maximum number of 1-link parts he can have is 2B: 
 * B from the broken links themselves, and B from the remaining parts of the 
 * chain being size 1.
 * 
 * This leaves one part of size L = N - 2B.
 * He can pay for 2B days using the 2B parts of size 1.
 * On the next day (Day 2B+1), he pays with the large part of size L. 
 * He gets back the 2B links he already paid for as change.
 * Finally, he can use these 2B links to pay for another 2B days.
 * 
 * So the total number of days N he can cover with B broken links is:
 * N = 2B (initial) + 1 (the large part day) + 2B (the change) = 4B + 1.
 * 
 * Therefore, to find the minimum B for a given N:
 * N <= 4B + 1
 * N - 1 <= 4B
 * B >= (N - 1) / 4
 * 
 * Taking the ceiling: B = ceil((N - 1) / 4).
 * For N=1, B=0.
 * For N=5, B = ceil(4/4) = 1.
 * For N=9, B = ceil(8/4) = 2.
 * In integer arithmetic, B = (N - 1 + 3) / 4 = (N + 2) / 4.
 * Let's check N=1: (1+2)/4 = 0. Correct.
 * Let's check N=5: (5+2)/4 = 1. Correct.
 * Let's check N=9: (9+2)/4 = 2. Correct.
 * Let's check N=6: (6+2)/4 = 2. Correct.
 * 
 * The input N can be up to 10^16, so we use long long.
 */

using namespace std;

int main() {
    // Optimize input/output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long N;
    if (!(cin >> N)) return 0;

    if (N == 1) {
        cout << 0 << endl;
    } else {
        // The formula B = (N + 2) / 4 handles N=1 correctly (0) 
        // and matches the derived B = ceil((N-1)/4.0).
        // For N=1: (1+2)/4 = 0.
        // For N=2: (2+2)/4 = 1.
        // For N=5: (5+2)/4 = 1.
        // For N=9: (9+2)/4 = 2.
        long long B = (N + 2) / 4;
        cout << B << endl;
    }

    return 0;
}

