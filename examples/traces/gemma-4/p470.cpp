#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

typedef long long ll;

ll count_parity(ll L, ll R, int p) {
    if (L > R) return 0;
    auto odds_upto = [](ll K) { return (K + 1) / 2; };
    auto evens_upto = [](ll K) { return K / 2; };
    if (p == 1) return odds_upto(R) - odds_upto(L - 1);
    else return evens_upto(R) - evens_upto(L - 1);
}

vector<vector<ll>> get_counts(ll L) {
    vector<vector<ll>> cnt(2, vector<ll>(2, 0));
    if (L == 1) {
        cnt[0][1 % 2]++;
    } else {
        cnt[1][1 % 2]++;
        cnt[1][L % 2]++;
        if (L > 2) {
            cnt[0][0] += count_parity(2, L - 1, 0);
            cnt[0][1] += count_parity(2, L - 1, 1);
        }
    }
    return cnt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll X, Y, Z;
    int case_num = 1;
    while (cin >> X >> Y >> Z) {
        ll M = (X - 1) * Y * Z + X * (Y - 1) * Z + X * Y * (Z - 1);
        if (M == 0) {
            cout << "Case #" << case_num++ << ": 0" << endl;
            continue;
        }

        auto cX = get_counts(X);
        auto cY = get_counts(Y);
        auto cZ = get_counts(Z);

        ll O_odd = 0, O_even = 0;
        for (int px = 0; px < 2; ++px) {
            for (int sx = 0; sx < 2; ++sx) {
                if (cX[px][sx] == 0) continue;
                for (int py = 0; py < 2; ++py) {
                    for (int sy = 0; sy < 2; ++sy) {
                        if (cY[py][sy] == 0) continue;
                        for (int pz = 0; pz < 2; ++pz) {
                            for (int sz = 0; sz < 2; ++sz) {
                                if (cZ[pz][sz] == 0) continue;
                                if ((px ^ py ^ pz) == 1) {
                                    ll total = cX[px][sx] * cY[py][sy] * cZ[pz][sz];
                                    if ((sx ^ sy ^ sz) == 1) O_odd += total;
                                    else O_even += total;
                                }
                            }
                        }
                    }
                }
            }
        }

        ll ans = M;
        if (O_odd > 0 && O_even > 0) {
            ans += max(O_odd - 1, O_even - 1);
        } else if (O_odd > 0) {
            ans += O_odd - 2;
        } else if (O_even > 0) {
            ans += O_even - 2;
        }
        cout << "Case #" << case_num++ << ": " << ans << endl;
    }
    return 0;
}

