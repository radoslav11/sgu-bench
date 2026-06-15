#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long double AB, AC, AD, BC, BD, CD;
    if (!(cin >> AB >> AC >> AD >> BC >> BD >> CD)) return 0;

    long double xC = (AB * AB + AC * AC - BC * BC) / (2.0L * AB);
    long double yC2 = AC * AC - xC * xC;
    if (yC2 < 0 && yC2 > -1e-12L) yC2 = 0;
    long double yC = sqrt(max((long double)0, yC2));

    long double xD = (AB * AB + AD * AD - BD * BD) / (2.0L * AB);
    long double yD = (AC * AC + AD * AD - CD * CD - 2.0L * xC * xD) / (2.0L * yC);

    long double zD2 = AD * AD - xD * xD - yD * yD;
    if (zD2 < 0 && zD2 > -1e-12L) zD2 = 0;
    long double zD = sqrt(max((long double)0, zD2));

    long double volume = AB * yC * zD / 6.0L;

    cout << fixed << setprecision(4) << (double)volume << '\n';

    return 0;
}
