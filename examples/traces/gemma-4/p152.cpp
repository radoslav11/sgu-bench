#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem Analysis:
 * We are given N candidates and the number of votes Ai for each candidate.
 * Let S = sum(Ai). The real percentage of votes for each candidate is Ri = (Ai / S) * 100.
 * We need to find an integer part Pi for each candidate such that:
 * 1. Pi is either floor(Ri) or ceil(Ri).
 * 2. The total sum of all Pi equals 100.
 * 
 * Solution Approach:
 * Let Li = floor(Ri) = floor(100 * Ai / S).
 * Let Ui = ceil(Ri). If Ri is an integer, Li = Ui. Otherwise, Ui = Li + 1.
 * Let Di = 1 if Ri is not an integer, and Di = 0 if Ri is an integer.
 * 
 * We need to choose Pi in {Li, Li + Di} such that sum(Pi) = 100.
 * sum(Pi) = sum(Li + di) = sum(Li) + sum(di), where di is either 0 or Di.
 * We need sum(di) = 100 - sum(Li).
 * Let K = 100 - sum(Li).
 * We must choose exactly K indices where Di = 1 and set di = 1, and set di = 0 for the rest.
 * 
 * Since sum(Li) <= sum(Ri) = 100 and sum(Ui) >= sum(Ri) = 100, 
 * we are guaranteed that 0 <= K <= (number of indices where Di = 1),
 * as long as S > 0.
 * 
 * If S = 0, the percentage is undefined. However, based on the problem description,
 * we can assume S > 0.
 */

int main() {
    // Use fast I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    vector<long long> A(N);
    long long S = 0;
    for (int i = 0; i < N; ++i) {
        if (!(cin >> A[i])) break;
        S += A[i];
    }

    // If there are no votes at all, it's impossible to have percentages adding to 100.
    if (S == 0) {
        cout << "No solution" << endl;
        return 0;
    }

    vector<long long> L(N);
    vector<int> D(N);
    long long total_L = 0;
    int count_D1 = 0;

    for (int i = 0; i < N; ++i) {
        // Ri = (100 * A[i]) / S. floor(Ri) is integer division.
        L[i] = (100 * A[i]) / S;
        // Di is 1 if Ri is not an integer, meaning 100*A[i] % S != 0
        D[i] = (100 * A[i] % S != 0) ? 1 : 0;
        total_L += L[i];
        if (D[i]) {
            count_D1++;
        }
    }

    // We need K candidates to have Pi = Li + 1.
    long long K = 100 - total_L;

    // As derived, 0 <= K <= count_D1 should always hold if S > 0.
    if (K < 0 || K > count_D1) {
        cout << "No solution" << endl;
    } else {
        for (int i = 0; i < N; ++i) {
            if (D[i] == 1 && K > 0) {
                cout << L[i] + 1;
                K--;
            } else {
                cout << L[i];
            }
            if (i < N - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }

    return 0;
}

