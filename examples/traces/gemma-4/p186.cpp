#include <iostream>
#include <vector>

/**
 * Problem: 186. The Chain
 * 
 * We are given N chains of varying lengths L1, L2, ..., LN.
 * The goal is to connect all these chains into one single long chain.
 * 
 * Actions allowed in 1 minute:
 * 1. Unchain one link.
 * 2. Remove or put into the unchained link some other links of any chain.
 * 3. Chain the link.
 * 
 * From the sample and analysis:
 * - The combination of these three actions can be thought of as: 
 *   Taking a link from an existing chain and using it to connect two other chains.
 *   This operation takes 1 minute and reduces the number of chains.
 * 
 * Let's define:
 * - M: current number of chains.
 * - K: number of chains that have only 1 link (these are "ready-to-use" single links).
 * 
 * Strategy:
 * - If we have a 1-link chain (K > 0) and there are more than 2 chains total (M > 2), 
 *   we can use that 1-link chain to connect two other chains. 
 *   This takes 1 minute, M decreases by 2, and K decreases by 1.
 * - If K = 0 and M > 2, we must unchain a link from an existing chain (of length > 1).
 *   This takes 1 minute, M decreases by 1, and we gain a 1-link chain (K becomes 1).
 * - If M = 2, we can connect them in 1 minute regardless of whether K=0 or K>0.
 * 
 * Implementation using a simple simulation approach.
 */

int main() {
    // Optimize input/output performance
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N;
    if (!(std::cin >> N)) return 0;

    int M = N; // Current number of chains
    int K = 0; // Number of chains with length 1
    for (int i = 0; i < N; ++i) {
        int L;
        std::cin >> L;
        if (L == 1) {
            K++;
        }
    }

    // If there's only 1 chain, no action is needed.
    if (M <= 1) {
        std::cout << 0 << std::endl;
        return 0;
    }

    int minutes = 0;
    while (M > 1) {
        if (K > 0 && M > 2) {
            // Use a 1-link chain to connect two other chains.
            // M decreases by 2 (two chains joined), K decreases by 1 (the used 1-link chain).
            M -= 2;
            K -= 1;
            minutes++;
        } else if (M > 2) {
            // No 1-link chains available to connect others, 
            // so we must unchain a link from one of the existing chains.
            // This unchained link becomes a 1-link chain, and the original chain is reduced.
            // The total number of chains effectively decreases by 1 because the unchained 
            // link can be used immediately in the same process to join two others.
            // However, it is safer to model it as M -> M-1 and K=1.
            M -= 1;
            K = 1;
            minutes++;
        } else {
            // Only 2 chains left (M=2).
            // Connect them in one final step.
            minutes++;
            M = 1;
        }
    }

    std::cout << minutes << std::endl;

    return 0;
}

