#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Problem: Debt
 * The goal is to distribute N crystals among three creditors (Popov, Parkin, Studnev) 
 * to satisfy their debt requirements (P, O, S burcrystals).
 * Each crystal can be estimated as 'B' (2 burcrystals) or 'S' (1 burcrystal) 
 * by each of the creditors.
 * 
 * Strategy:
 * 1. There are only 8 possible types of crystals based on (vP, vO, vS) where each v_i in {1, 2}.
 * 2. Initially, assign each crystal to the creditor for whom it provides the maximum value.
 * 3. While any creditor's debt is not satisfied, pick a move (moving a crystal of some type 
 *    from one creditor to another) that increases the unsatisfied debt with minimum "loss"
 *    (difference in the value the crystal provides to the current vs new creditor).
 * 4. Since there are only 8 types and 3 creditors, we can efficiently find the best move.
 * 5. If all debts are satisfied, output the assignment; otherwise, output "no solution".
 */

struct Crystal {
    int v[3]; // v[0]=vP, v[1]=vO, v[2]=vS
};

int main() {
    // Optimize standard I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int P_target, O_target, S_target;
    if (!(cin >> P_target >> O_target >> S_target)) return 0;

    int N;
    if (!(cin >> N)) return 0;

    vector<Crystal> crystals(N);
    // x[vp][vo][vs][creditor] stores the number of crystals of type (vp, vo, vs) assigned to creditor
    int x[3][3][3][3] = {0}; 

    for (int i = 0; i < N; ++i) {
        string s;
        cin >> s;
        crystals[i].v[0] = (s[0] == 'B' ? 2 : 1);
        crystals[i].v[1] = (s[1] == 'B' ? 2 : 1);
        crystals[i].v[2] = (s[2] == 'B' ? 2 : 1);
    }

    // Initial greedy assignment
    for (int i = 0; i < N; ++i) {
        int best_c = 0;
        int max_v = crystals[i].v[0];
        // Tie-breaking rule: P > O > S
        if (crystals[i].v[1] > max_v) {
            max_v = crystals[i].v[1];
            best_c = 1;
        }
        if (crystals[i].v[2] > max_v) {
            max_v = crystals[i].v[2];
            best_c = 2;
        }
        x[crystals[i].v[0]][crystals[i].v[1]][crystals[i].v[2]][best_c]++;
    }

    // Calculate initial debt satisfaction
    long long V[3] = {0, 0, 0};
    for (int vp = 1; vp <= 2; ++vp) {
        for (int vo = 1; vo <= 2; ++vo) {
            for (int vs = 1; vs <= 2; ++vs) {
                for (int c = 0; c < 3; ++c) {
                    int val_c = (c == 0 ? vp : (c == 1 ? vo : vs));
                    V[c] += (long long)x[vp][vo][vs][c] * val_c;
                }
            }
        }
    }

    int targets[3] = {P_target, O_target, S_target};

    // Greedy adjustment loop
    while (true) {
        int bvp = -1, bvo = -1, bvs = -1;
        int bc1 = -1, bc2 = -1;
        int min_cost = 10; // Any cost will be in range [-1, 1]

        for (int vp = 1; vp <= 2; ++vp) {
            for (int vo = 1; vo <= 2; ++vo) {
                for (int vs = 1; vs <= 2; ++vs) {
                    for (int c1 = 0; c1 < 3; ++c1) {
                        if (x[vp][vo][vs][c1] > 0) {
                            for (int c2 = 0; c2 < 3; ++c2) {
                                if (c1 != c2 && V[c2] < targets[c2]) {
                                    int v1 = (c1 == 0 ? vp : (c1 == 1 ? vo : vs));
                                    int v2 = (c2 == 0 ? vp : (c2 == 1 ? vo : vs));
                                    int cost = v1 - v2;
                                    if (cost < min_cost) {
                                        min_cost = cost;
                                        bvp = vp; bvo = vo; bvs = vs;
                                        bc1 = c1; bc2 = c2;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (bc1 == -1) break;

        // Update counts and debt values for the selected move
        x[bvp][bvo][bvs][bc1]--;
        x[bvp][bvo][bvs][bc2]++;
        V[bc1] -= (bc1 == 0 ? bvp : (bc1 == 1 ? bvo : bvs));
        V[bc2] += (bc2 == 0 ? bvp : (bc2 == 1 ? bvo : bvs));
    }

    // Check if solution is found
    if (V[0] >= P_target && V[1] >= O_target && V[2] >= S_target) {
        // To reconstruct the output string, collect indices for each type
        vector<int> type_indices[3][3][3];
        for (int i = 0; i < N; ++i) {
            type_indices[crystals[i].v[0]][crystals[i].v[1]][crystals[i].v[2]].push_back(i);
        }
        string assignment(N, ' ');
        char c_chars[3] = {'P', 'O', 'S'};
        for (int vp = 1; vp <= 2; ++vp) {
            for (int vo = 1; vo <= 2; ++vo) {
                for (int vs = 1; vs <= 2; ++vs) {
                    int idx = 0;
                    for (int c = 0; c < 3; ++c) {
                        for (int k = 0; k < x[vp][vo][vs][c]; ++k) {
                            assignment[type_indices[vp][vo][vs][idx++]] = c_chars[c];
                        }
                    }
                }
            }
        }
        cout << assignment << endl;
    } else {
        cout << "no solution" << endl;
    }

    return 0;
}

