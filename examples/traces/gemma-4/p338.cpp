#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

double W_opt[23][23];
double P_BM[23];
bool bm_stops[23];
double stop_BM[23];

int main() {
    int X;
    if (!(cin >> X)) return 0;

    for (int sp = 0; sp <= 21; ++sp) {
        for (int sc = 21; sc >= 0; --sc) {
            double stop_val = (sc >= sp) ? 1.0 : 0.0;
            double roll_val = 0;
            for (int d = 1; d <= 6; ++d) {
                if (sc + d <= 21) roll_val += W_opt[sc + d][sp];
            }
            W_opt[sc][sp] = max(stop_val, roll_val / 6.0);
        }
    }

    for (int s = 21; s >= 0; --s) {
        double stop_prob = 1.0 - W_opt[0][s];
        double roll_prob = 0;
        for (int d = 1; d <= 6; ++d) {
            if (s + d <= 21) roll_prob += P_BM[s + d];
        }
        roll_prob /= 6.0;
        if (stop_prob > roll_prob + 1e-13) {
            P_BM[s] = stop_prob;
            bm_stops[s] = true;
        } else {
            P_BM[s] = roll_prob;
            bm_stops[s] = false;
        }
    }

    double reach_BM[23] = {0};
    reach_BM[0] = 1.0;
    for (int s = 0; s <= 21; ++s) {
        if (bm_stops[s]) {
            stop_BM[s] = reach_BM[s];
        } else {
            for (int d = 1; d <= 6; ++d) {
                if (s + d <= 21) reach_BM[s + d] += reach_BM[s] / 6.0;
            }
        }
    }

    double total_prob_win = 0;
    for (int sb = 0; sb <= 21; ++sb) {
        if (stop_BM[sb] <= 0) continue;

        vector<double> P_win_if_stop(22);
        for (int sa = 0; sa <= 21; ++sa) {
            vector<double> G(23, 0.0), E_winA(23, 0.0);
            for (int sc = 21; sc >= 0; --sc) {
                double stop_v = ((sc < sa) ? 1.0 : 0.0) + (double)X * ((sc < sb) ? 1.0 : 0.0);
                double roll_g = 0, roll_e = 0;
                for (int d = 1; d <= 6; ++d) {
                    if (sc + d <= 21) {
                        roll_g += G[sc + d];
                        roll_e += E_winA[sc + d];
                    }
                }
                roll_g /= 6.0; roll_e /= 6.0;
                if (stop_v < roll_g - 1e-13) {
                    G[sc] = stop_v;
                    E_winA[sc] = (sc < sa) ? 1.0 : 0.0;
                } else {
                    G[sc] = roll_g;
                    E_winA[sc] = roll_e;
                }
            }
            P_win_if_stop[sa] = E_winA[0];
        }

        vector<double> P_winA_sa(23, 0.0);
        for (int sa = 21; sa >= 0; --sa) {
            double stop_p = P_win_if_stop[sa];
            double roll_p = 0;
            for (int d = 1; d <= 6; ++d) {
                if (sa + d <= 21) roll_p += P_winA_sa[sa + d];
            }
            roll_p /= 6.0;
            P_winA_sa[sa] = max(stop_p, roll_p);
        }

        double prob_win_sb = 0;
        vector<double> reach_A(23, 0.0);
        reach_A[0] = 1.0;
        for (int sa = 0; sa <= 21; ++sa) {
            double stop_p = P_win_if_stop[sa];
            double roll_p = 0;
            for (int d = 1; d <= 6; ++d) {
                if (sa + d <= 21) roll_p += P_winA_sa[sa + d];
            }
            roll_p /= 6.0;

            if (stop_p > roll_p + 1e-13) {
                prob_win_sb += reach_A[sa] * stop_p;
            } else {
                for (int d = 1; d <= 6; ++d) {
                    if (sa + d <= 21) reach_A[sa + d] += reach_A[sa] / 6.0;
                }
            }
        }
        total_prob_win += stop_BM[sb] * prob_win_sb;
    }

    cout << fixed << setprecision(6) << total_prob_win << endl;
    return 0;
}

