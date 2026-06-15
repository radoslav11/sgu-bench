#include <bits/stdc++.h>
using namespace std;

int main(){
    string A;
    long long B;
    cin >> A >> B;
    
    if(B == 1){
        cout << A << endl;
        return 0;
    }
    
    // Convert A to base B
    // A is big number, do repeated division
    vector<long long> digits; // base B digits, least significant first
    // divide big number by B repeatedly
    string num = A;
    while(num != "0" && !num.empty()){
        // compute num % B and num / B
        long long rem = 0;
        string next = "";
        for(char c : num){
            rem = rem * 10 + (c - '0');
            long long q = rem / B;
            rem = rem % B;
            if(!next.empty() || q > 0) next += (char)('0' + q); // won't work for q>9
            // need proper handling
        }
        // Redo with proper quotient
        rem = 0; next = "";
        for(char c : num){
            rem = rem * 10 + (c-'0');
            long long q = rem / B;
            rem %= B;
            if(!next.empty() || q > 0){
                // q can be multi-digit? No, q < 10 since rem < B*10 <= 10^7
                next += to_string(q);
            }
        }
        digits.push_back(rem);
        if(next.empty()) num = "0";
        else num = next;
    }
    
    long long carry = 0, ans = 0;
    for(int i = 0; i < (int)digits.size() || carry != 0; i++){
        long long d = (i < (int)digits.size() ? digits[i] : 0) + carry;
        long long r = ((d % B) + B) % B;
        long long c = r;
        if(c > B/2) c -= B;
        if(B % 2 == 0 && c == -(B/2)) c = B/2; // take positive
        ans += abs(c);
        carry = (d - c) / B;
    }
    cout << ans << endl;
}
