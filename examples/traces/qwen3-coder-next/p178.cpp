#include <iostream>
using namespace std;

int main() {
    long long n;
    cin >> n;
    long long k = 0;
    long long bound = 2; // 2 * (4^0)
    while (bound < n + 1) {
        k++;
        bound *= 4;
    }
    cout << k << endl;
    return 0;
}
