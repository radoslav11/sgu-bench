#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    int h_min = (n + 1) / 14;
    if (h_min == 0) {
        h_min = 1;
    }
    int h_max = (n - 2) / 14;
    if (h_min <= h_max) {
        cout << h_min << endl;
    } else {
        cout << -1 << endl;
    }
    return 0;
}
