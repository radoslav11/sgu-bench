#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    double n, m;
    if (cin >> n >> m) {
        double expected = n * (1.0 - pow(1.0 - 1.0 / n, m));
        cout << fixed << setprecision(10) << expected << "\n";
    }
    
    return 0;
}
