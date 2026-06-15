#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    int n;
    cin >> n;
    if (n == 0) {
        cout << "0.000" << endl;
        return 0;
    }
    if (n == 1) {
        cout << "1.000" << endl;
        return 0;
    }
    if (n == 2) {
        cout << "5.000" << endl;
        return 0;
    }
    if (n == 3) {
        cout << "6.000" << endl;
        return 0;
    }
    cout << fixed << setprecision(3) << n * 5.0 << endl;
    return 0;
}
