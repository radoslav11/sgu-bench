#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

int n;
vector<double> x;

double eval(double D, double &best_med) {
    vector<double> z(n);
    for (int i = 0; i < n; ++i) {
        z[i] = x[i] - i * D;
    }
    vector<double> temp = z;
    sort(temp.begin(), temp.end());
    double median = temp[n / 2];
    double cost = 0;
    for (int i = 0; i < n; ++i) {
        cost += abs(z[i] - median);
    }
    best_med = median;
    return cost;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> n)) return 0;
    x.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> x[i];
    }

    double l = 0.0, r = 40000.0;
    double best_D = 0.0;
    double min_cost = 1e18;
    double best_median = 0.0;

    for (int iter = 0; iter < 100; ++iter) {
        double m1 = l + (r - l) / 3.0;
        double m2 = r - (r - l) / 3.0;
        double med1, med2;
        double c1 = eval(m1, med1);
        double c2 = eval(m2, med2);
        
        if (c1 < c2) {
            r = m2;
            if (c1 < min_cost) {
                min_cost = c1;
                best_D = m1;
                best_median = med1;
            }
        } else if (c1 > c2) {
            l = m1;
            if (c2 < min_cost) {
                min_cost = c2;
                best_D = m2;
                best_median = med2;
            }
        } else {
            l = m1;
            r = m2;
            if (c1 < min_cost) {
                min_cost = c1;
                best_D = m1;
                best_median = med1;
            }
        }
    }
    
    double final_D = (l + r) / 2.0;
    double final_med;
    double final_cost = eval(final_D, final_med);
    if (final_cost < min_cost) {
        min_cost = final_cost;
        best_D = final_D;
        best_median = final_med;
    }

    cout << fixed << setprecision(4) << min_cost << "\n";
    for (int i = 0; i < n; ++i) {
        cout << fixed << setprecision(10) << best_median + i * best_D << (i == n - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}
