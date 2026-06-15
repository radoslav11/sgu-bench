#include <bits/stdc++.h>
using namespace std;

int main() {
    string line;
    while (getline(cin, line)) {
        if (line.find("Test") != string::npos) {
            continue;
        }
        istringstream iss(line);
        int S, P;
        if (iss >> S >> P) {
            int diff = P - S;
            int cups = 0;
            if (diff >= 1800) cups = 4;
            else if (diff >= 900) cups = 3;
            else if (diff >= 300) cups = 2;
            else if (diff >= 1) cups = 1;
            else cups = 0;
            cout << cups << "\n";
        }
    }
    return 0;
}
