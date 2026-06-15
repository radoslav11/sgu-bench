#include <iostream>
using namespace std;

int main() {
    long long L;
    cin >> L;
    long long n = (L + 1) / 2;
    if (n % 3 == 2) {
        cout << n - 1 << endl;
    } else {
        cout << n << endl;
    }
    return 0;
}
