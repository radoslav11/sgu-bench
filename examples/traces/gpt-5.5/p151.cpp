#include <bits/stdc++.h>
using namespace std;

long long parse100(const string& s) {
    long long intpart = 0, frac = 0;
    int i = 0, n = (int)s.size();

    while (i < n && s[i] != '.') {
        intpart = intpart * 10 + (s[i] - '0');
        i++;
    }

    if (i < n && s[i] == '.') {
        i++;
        int cnt = 0;
        while (i < n && cnt < 2) {
            frac = frac * 10 + (s[i] - '0');
            i++;
            cnt++;
        }
        while (cnt < 2) {
            frac *= 10;
            cnt++;
        }
    }

    return intpart * 100 + frac;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string sc, sb, sm;
    cin >> sc >> sb >> sm;

    long long ci = parse100(sc);
    long long bi = parse100(sb);
    long long mi = parse100(sm);

    // Exact sign check for:
    // x = (c^2 - b^2) / (4m)
    // y^2 = c^2 - (m + x)^2
    //
    // In scaled-by-100 coordinates:
    // X = x * 100 = (ci^2 - bi^2) / (4mi)
    // y_scaled^2 = ci^2 - (mi + X)^2
    //
    // Multiply by (4mi)^2 to avoid fractions.
    __int128 denom = (__int128)4 * mi;
    __int128 diff = (__int128)ci * ci - (__int128)bi * bi;
    __int128 t = (__int128)mi * denom + diff;
    __int128 num = (__int128)ci * ci * denom * denom - t * t;

    if (num <= 0) {
        cout << "Mission impossible\n";
        return 0;
    }

    long double c = (long double)ci / 100.0L;
    long double b = (long double)bi / 100.0L;
    long double m = (long double)mi / 100.0L;

    long double x = (c * c - b * b) / (4.0L * m);
    long double y2 = c * c - (m + x) * (m + x);

    if (y2 <= 0) {
        cout << "Mission impossible\n";
        return 0;
    }

    long double y = sqrtl(y2);

    long double ax = 0.0L, ay = 0.0L;
    long double bx = m + x, by = y;
    long double cx = m - x, cy = -y;

    cout << fixed << setprecision(5);
    cout << (double)ax << ' ' << (double)ay << '\n';
    cout << (double)bx << ' ' << (double)by << '\n';
    cout << (double)cx << ' ' << (double)cy << '\n';

    return 0;
}
