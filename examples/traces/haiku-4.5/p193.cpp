#include <bits/stdc++.h>
using namespace std;

int main(){
    string n;
    cin >> n;
    
    // Check if n is odd
    if ((n.back() - '0') % 2 == 1) {
        // n is odd, answer is n/2 (integer division)
        int carry = 0;
        for (int i = 0; i < n.length(); i++) {
            int digit = (n[i] - '0') + carry * 10;
            if (i > 0) cout << digit / 2;
            else if (digit / 2 > 0) cout << digit / 2;
            carry = digit % 2;
        }
        cout << endl;
    } else {
        // n is even
        // Check if n/2 is odd (i.e., n ≡ 2 (mod 4))
        int secondLast = (n[n.length()-2] - '0');
        if (secondLast % 2 == 1) {
            // n/2 is odd, answer is n/2
            int carry = 0;
            for (int i = 0; i < n.length(); i++) {
                int digit = (n[i] - '0') + carry * 10;
                if (i > 0 || digit / 2 > 0) cout << digit / 2;
                carry = digit % 2;
            }
            cout << endl;
        } else {
            // n/2 is even, answer is 1
            cout << 1 << endl;
        }
    }
    
    return 0;
}
