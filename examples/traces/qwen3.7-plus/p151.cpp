#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

long long parse_to_int(const string& s) {
    long long res = 0;
    int decimal_places = 0;
    bool after_dot = false;
    for (char c : s) {
        if (c == '.') {
            after_dot = true;
        } else if (c >= '0' && c <= '9') {
            res = res * 10 + (c - '0');
            if (after_dot) {
                decimal_places++;
            }
        }
    }
    while (decimal_places < 2) {
        res *= 10;
        decimal_places++;
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string s_c, s_b, s_m;
    if (!(cin >> s_c >> s_b >> s_m)) return 0;
    
    __int128 C = parse_to_int(s_c);
    __int128 B = parse_to_int(s_b);
    __int128 M = parse_to_int(s_m);
    
    __int128 B2 = B * B;
    __int128 C2 = C * C;
    __int128 M2 = M * M;
    __int128 B4 = B2 * B2;
    __int128 C4 = C2 * C2;
    __int128 M4 = M2 * M2;
    
    __int128 D_int = 2 * B2 * C2 + 8 * M2 * (B2 + C2) - 16 * M4 - B4 - C4;
    
    if (D_int <= 0) {
        cout << "Mission impossible\n";
        return 0;
    }
    
    unsigned __int128 uD = D_int;
    unsigned long long high = (unsigned long long)(uD >> 64);
    unsigned long long low = (unsigned long long)(uD & 0xFFFFFFFFFFFFFFFFULL);
    long double d_val = (long double)high * 18446744073709551616.0L + (long double)low;
    
    long double X = sqrt(d_val) / (4.0 * (long double)M);
    long double Y = (long double)(B2 - C2) / (4.0 * (long double)M);
    
    long double x = X / 100.0;
    long double y = Y / 100.0;
    long double m = (long double)M / 100.0;
    
    if (fabsl(x) < 1e-7) x = 0.0;
    if (fabsl(y) < 1e-7) y = 0.0;
    
    cout << fixed << setprecision(5);
    cout << 0.0 << " " << m << "\n";
    cout << -x << " " << y << "\n";
    cout << x << " " << -y << "\n";
    
    return 0;
}
