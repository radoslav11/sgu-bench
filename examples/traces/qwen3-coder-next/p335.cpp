#include <iostream>
#include <cmath>
using namespace std;

int main() {
    long long H, W;
    cin >> H >> W;
    long long Rc, Cc, Rt, Ct;
    char turn;
    cin >> Rc >> Cc >> Rt >> Ct;
    cin >> turn;

    long long d = abs(Rc - Rt) + abs(Cc - Ct);

    if (H == 2 && W == 2) {
        if (d == 1) {
            if (turn == 'C') {
                cout << 1 << endl;
            } else {
                cout << 0 << endl;
            }
        } else {
            if (turn == 'C') {
                cout << 0 << endl;
            } else {
                cout << 2 << endl;
            }
        }
    } else {
        if (turn == 'C') {
            cout << d << endl;
        } else {
            if (d % 2 == 0) {
                cout << d << endl;
            } else {
                cout << d + 1 << endl;
            }
        }
    }

    return 0;
}
