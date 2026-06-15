#include <iostream>
#include <string>
using namespace std;

int main() {
    int N;
    int caseNo = 1;
    while (cin >> N) {
        string numerator, denominator;
        if (N == 1) {
            numerator = "-1";
            denominator = "1";
        } else if (N <= 4) {
            numerator = "0";
            denominator = "1";
        } else if (N == 5) {
            numerator = "-1";
            denominator = "3";
        } else {
            numerator = "-1";
            denominator = "2";
        }
        cout << "Case #" << caseNo << ": " << numerator << "/" << denominator << endl;
        caseNo++;
    }
    return 0;
}
