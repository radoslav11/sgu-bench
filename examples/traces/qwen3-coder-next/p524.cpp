#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<double> x(n);
    for (int i = 0; i < n; i++) {
        cin >> x[i];
    }

    double d = (x[n-1] - x[0]) / (n - 1);
    double total_shift = 0.0;
    vector<double> y(n);
    for (int i = 0; i < n; i++) {
        y[i] = x[0] + i * d;
        total_shift += abs(x[i] - y[i]);
    }

    cout << fixed << setprecision(4) << total_shift << endl;
    cout << fixed << setprecision(7);
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << " ";
        cout << y[i];
    }
    cout << endl;

    return 0;
}
