#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    long long a, b;
    cin >> a >> b;
    int n;
    cin >> n;
    vector<long long> notes(n);
    for (int i = 0; i < n; i++) {
        cin >> notes[i];
    }

    long long x = 0; // total boys joined so far
    long long y = 0; // total girls left so far

    for (int i = 0; i < n; i++) {
        long long t = notes[i];
        // Calculate the lower bound for x_new
        long long L = max({x, y + t - b, 0LL});
        // Calculate the upper bound for x_new: min(a, t)
        long long R = min(a, t);

        if (L > R) {
            cout << "ERROR" << endl;
            return 0;
        }

        long long x_new = L;
        long long y_new = x_new - (t - b); // which is x_new + b - t

        long long dx = x_new - x;
        long long dy = y_new - y;

        cout << dx << " " << dy << endl;

        x = x_new;
        y = y_new;
    }

    return 0;
}
