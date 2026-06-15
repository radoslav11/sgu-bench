#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

int n;
int a[15];
bool possible[1024][1005];
vector<int> ach[1024];
double min_area = 1e18;

void search(int idx, int m1, int m2, int m3) {
    if (idx == n) {
        if (m1 == 0 || m2 == 0 || m3 == 0) return;
        const auto& v1 = ach[m1];
        const auto& v2 = ach[m2];
        const auto& v3 = ach[m3];
        for (int L1 : v1) {
            for (int L2 : v2) {
                int low = abs(L1 - L2);
                int high = L1 + L2;
                
                auto it_low = lower_bound(v3.begin(), v3.end(), low);
                if (it_low != v3.end() && *it_low <= high) {
                    double A = L1, B = L2, C = *it_low;
                    double p = (A + B + C) / 2.0;
                    double area = sqrt(max(0.0, p * (p - A) * (p - B) * (p - C)));
                    if (area < min_area) min_area = area;
                }
                
                auto it_high = upper_bound(v3.begin(), v3.end(), high);
                if (it_high != v3.begin()) {
                    auto max_valid = prev(it_high);
                    if (*max_valid >= low) {
                        double A = L1, B = L2, C = *max_valid;
                        double p = (A + B + C) / 2.0;
                        double area = sqrt(max(0.0, p * (p - A) * (p - B) * (p - C)));
                        if (area < min_area) min_area = area;
                    }
                }
            }
        }
        return;
    }
    search(idx + 1, m1 | (1 << idx), m2, m3);
    search(idx + 1, m1, m2 | (1 << idx), m3);
    search(idx + 1, m1, m2, m3 | (1 << idx));
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> n)) return 0;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    bool can_zero = false;
    for (int mask = 0; mask < (1 << (n - 1)); ++mask) {
        int sum1 = 0, sum2 = 0;
        for (int i = 0; i < n; ++i) {
            if ((mask >> i) & 1) sum1 += a[i];
            else sum2 += a[i];
        }
        if (sum1 == sum2) {
            can_zero = true;
            break;
        }
    }
    
    if (can_zero) {
        cout << fixed << setprecision(10) << 0.0 << "\n";
        return 0;
    }
    
    possible[0][0] = true;
    for (int mask = 1; mask < (1 << n); ++mask) {
        int i = __builtin_ctz(mask);
        int prev = mask ^ (1 << i);
        int v = a[i];
        for (int s = 0; s <= 1000; ++s) {
            if (possible[prev][s]) {
                if (s + v <= 1000) possible[mask][s + v] = true;
                possible[mask][abs(s - v)] = true;
            }
        }
    }
    
    for (int mask = 0; mask < (1 << n); ++mask) {
        for (int s = 0; s <= 1000; ++s) {
            if (possible[mask][s]) {
                ach[mask].push_back(s);
            }
        }
    }
    
    sort(a, a + n, greater<int>());
    search(0, 0, 0, 0);
    
    cout << fixed << setprecision(10) << min_area << "\n";
    return 0;
}
