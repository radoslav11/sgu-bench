#include <iostream>
using namespace std;

int main() {
    int S, P;
    cin >> S >> P;
    
    int delay = P - S;
    int cups = 0;
    
    if (delay <= 0) {
        cups = 0;
    } else if (delay < 300) {
        cups = 1;
    } else if (delay < 900) {
        cups = 2;
    } else if (delay < 1800) {
        cups = 3;
    } else {
        cups = 4;
    }
    
    cout << cups << endl;
    
    return 0;
}
