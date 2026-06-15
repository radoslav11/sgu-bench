#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

/**
 * Problem: Thiefs And Cops
 * 
 * The problem is a game played on an HxW grid.
 * Cop wants to minimize the number of moves to catch the thief,
 * and the thief wants to maximize it or avoid being caught.
 * 
 * Observations:
 * 1. In a 2D grid (H > 1 and W > 1):
 *    - If the cop moves first and the distance between them is 1, the cop catches the thief in 1 move.
 *    - If the thief moves first and the distance between them is 2, the cop can only catch the thief 
 *      if the thief is cornered (a corner cell) and the cop is at the diagonal cell.
 *      In this specific case, any move the thief makes will bring them to a distance of 1 from the cop,
 *      allowing the cop to catch them on the second move.
 *    - In all other cases where H > 1 and W > 1, the thief can always maneuver to avoid the cop.
 * 
 * 2. In a 1D grid (H = 1 or W = 1):
 *    - The cop can always eventually catch the thief.
 *    - The thief's optimal strategy is to move towards the furthest edge from the cop.
 *    - If c_c < c_t (cop is to the left of the thief), the thief moves towards W.
 *    - If c_c > c_t (cop is to the right of the thief), the thief moves towards 1.
 *    - Let's derive the move count for the 1D case.
 *    - For c_c < c_t, if C moves first:
 *      If d(C, T) = 1, the answer is 1.
 *      Otherwise, the answer is 2*(W - c_c) - (c_t - c_c).
 *    - For c_c < c_t, if T moves first:
 *      The answer is 2*(W - c_c) - (c_t - c_c).
 *    - Similar formulas apply when c_c > c_t.
 */

int main() {
    // Speed up I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long H, W;
    if (!(cin >> H >> W)) return 0;

    long long Rc, Cc, Rt, Ct;
    cin >> Rc >> Cc >> Rt >> Ct;

    char turn;
    cin >> turn;

    long long dist = abs(Rc - Rt) + abs(Cc - Ct);

    if (H == 1 || W == 1) {
        // 1D grid case
        if (H == 1) {
            // Grid is 1xW, movement occurs in columns
            if (Cc < Ct) {
                if (turn == 'C') {
                    if (dist == 1) cout << 1 << endl;
                    else cout << 2 * (W - Cc) - (Ct - Cc) << endl;
                } else {
                    cout << 2 * (W - Cc) - (Ct - Cc) << endl;
                }
            } else if (Cc > Ct) {
                if (turn == 'C') {
                    if (dist == 1) cout << 1 << endl;
                    else cout << 2 * (Cc - 1) - (Cc - Ct) << endl;
                } else {
                    cout << 2 * (Cc - 1) - (Cc - Ct) << endl;
                }
            } else {
                // This should not happen as initial positions differ
                cout << 0 << endl;
            }
        } else {
            // Grid is Hx1, movement occurs in rows
            if (Rc < Rt) {
                if (turn == 'C') {
                    if (dist == 1) cout << 1 << endl;
                    else cout << 2 * (H - Rc) - (Rt - Rc) << endl;
                } else {
                    cout << 2 * (H - Rc) - (Rt - Rc) << endl;
                }
            } else if (Rc > Rt) {
                if (turn == 'C') {
                    if (dist == 1) cout << 1 << endl;
                    else cout << 2 * (Rc - 1) - (Rc - Rt) << endl;
                } else {
                    cout << 2 * (Rc - 1) - (Rc - Rt) << endl;
                }
            } else {
                cout << 0 << endl;
            }
        }
    } else {
        // 2D grid case (H > 1 and W > 1)
        if (turn == 'C') {
            // Cop moves first
            if (dist == 1) cout << 1 << endl;
            else cout << 0 << endl;
        } else {
            // Thief moves first
            // Thief can be caught in move 2 only if they're cornered and the cop is diagonal.
            bool thief_is_corner = (Rt == 1 || Rt == H) && (Ct == 1 || Ct == W);
            bool cop_is_diagonal = abs(Rc - Rt) == 1 && abs(Cc - Ct) == 1;
            
            if (dist == 2 && thief_is_corner && cop_is_diagonal) {
                cout << 2 << endl;
            } else {
                cout << 0 << endl;
            }
        }
    }

    return 0;
}

