#include <bits/stdc++.h>
using namespace std;

int main() {
    int S, P;
    cin >> S >> P;

    int late = P - S;
    int cups = 0;

    if (late >= 1800) cups = 4;
    else if (late >= 900) cups = 3;
    else if (late >= 300) cups = 2;
    else if (late >= 1) cups = 1;

    cout << cups << '\n';
    return 0;
}
