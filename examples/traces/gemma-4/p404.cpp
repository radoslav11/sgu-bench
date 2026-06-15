#include <iostream>
#include <vector>
#include <string>

/**
 * Problem 404: Fortune-telling with camomile
 * 
 * The goal is to simulate picking N petals from a camomile and pronouncing 
 * phrases from a cyclic list of M phrases. Each petal picked corresponds 
 * to the next phrase in the list. After picking the N-th petal, the 
 * phrase pronounced at that step is the final answer.
 * 
 * Let the phrases be P[0], P[1], ..., P[M-1].
 * For the 1st petal, we say P[0].
 * For the 2nd petal, we say P[1].
 * ...
 * For the N-th petal, we say P[(N-1) % M].
 * 
 * Constraints:
 * 1 <= N <= 100
 * 1 <= M <= 100
 * Phrase length: 1 <= length <= 100
 * Time limit: 0.25s
 * Memory limit: 64MB
 * 
 * The problem statement says phrases consist only of latin letters, which 
 * implies there are no spaces within a phrase. Thus, `std::cin` is suitable 
 * for reading each phrase.
 */

using namespace std;

int main() {
    // Optimization to speed up input and output, though not strictly necessary
    // for this problem's small constraints.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    // Read the number of petals N and the number of phrases M.
    if (!(cin >> N >> M)) {
        return 0;
    }

    // A vector to store the M phrases.
    vector<string> phrases(M);
    for (int i = 0; i < M; ++i) {
        // Read each phrase from the input.
        if (!(cin >> phrases[i])) {
            break;
        }
    }

    // Ensure M is positive to avoid division by zero, though constraints say M >= 1.
    if (M > 0) {
        // Calculate the index of the phrase pronounced after the N-th petal.
        // Since the list is cyclic, the N-th phrase corresponds to (N-1) % M.
        int result_index = (N - 1) % M;
        // Output the resulting phrase.
        cout << phrases[result_index] << endl;
    }

    return 0;
}

