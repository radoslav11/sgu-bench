#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (!(cin >> n)) return 0;
    
    vector<pair<double, double>> m(n);
    for (int i = 0; i < n; i++) {
        cin >> m[i].first >> m[i].second;
    }
    
    if (n % 2 != 0) {
        cout << "YES\n";
        double vx = 0, vy = 0;
        for (int j = 0; j < n; j++) {
            if ((n - (j + 1)) % 2 == 0) {
                vx += m[j].first;
                vy += m[j].second;
            } else {
                vx -= m[j].first;
                vy -= m[j].second;
            }
        }
        vector<pair<double, double>> v(n);
        v[0] = {vx, vy};
        for (int j = 1; j < n; j++) {
            v[j] = {2 * m[j - 1].first - v[j - 1].first, 2 * m[j - 1].second - v[j - 1].second};
        }
        
        cout << fixed << setprecision(6);
        for (int j = 0; j < n; j++) {
            cout << v[j].first << " " << v[j].second << "\n";
        }
    } else {
        double sx = 0, sy = 0;
        for (int j = 0; j < n; j++) {
            if ((n - (j + 1)) % 2 == 0) {
                sx += m[j].first;
                sy += m[j].second;
            } else {
                sx -= m[j].first;
                sy -= m[j].second;
            }
        }
        if (abs(sx) > 1e-6 || abs(sy) > 1e-6) {
            cout << "NO\n";
        } else {
            cout << "YES\n";
            vector<pair<double, double>> v(n);
            v[0] = {0.0, 0.0};
            for (int j = 1; j < n; j++) {
                v[j] = {2 * m[j - 1].first - v[j - 1].first, 2 * m[j - 1].second - v[j - 1].second};
            }
            cout << fixed << setprecision(6);
            for (int j = 0; j < n; j++) {
                cout << v[j].first << " " << v[j].second << "\n";
            }
        }
    }
    
    return 0;
}
