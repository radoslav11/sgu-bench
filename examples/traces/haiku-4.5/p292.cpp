#include<bits/stdc++.h>
using namespace std;

// Function to divide a big integer (represented as string) by a regular integer
// Returns the quotient as a string
string divideBigInt(string num, long long divisor) {
    if (divisor == 0) return "0";
    
    long long remainder = 0;
    string result = "";
    
    for (char digit : num) {
        remainder = remainder * 10 + (digit - '0');
        result += char((remainder / divisor) + '0');
        remainder %= divisor;
    }
    
    // Remove leading zeros
    size_t pos = result.find_first_not_of('0');
    if (pos == string::npos) return "0";
    return result.substr(pos);
}

// Function to multiply two numbers represented as strings
string multiplyStrings(string num1, string num2) {
    int n1 = num1.size(), n2 = num2.size();
    if (n1 == 0 || n2 == 0) return "0";
    
    vector<int> result(n1 + n2, 0);
    
    int i_n1 = 0;
    int i_n2 = 0;
    
    for (int i = n1 - 1; i >= 0; i--) {
        int carry = 0;
        int n1_digit = num1[i] - '0';
        i_n2 = 0;
        
        for (int j = n2 - 1; j >= 0; j--) {
            int n2_digit = num2[j] - '0';
            int sum = n1_digit * n2_digit + result[i_n1 + i_n2] + carry;
            carry = sum / 10;
            result[i_n1 + i_n2] = sum % 10;
            i_n2++;
        }
        
        if (carry > 0)
            result[i_n1 + i_n2] += carry;
        i_n1++;
    }
    
    int i = result.size() - 1;
    while (i >= 0 && result[i] == 0)
        i--;
    
    if (i == -1) return "0";
    
    string s = "";
    while (i >= 0)
        s += to_string(result[i--]);
    
    return s;
}

// Compare two big integers represented as strings
// Returns: 1 if a > b, -1 if a < b, 0 if a == b
int compareBigInt(string a, string b) {
    if (a.length() != b.length())
        return a.length() > b.length() ? 1 : -1;
    return a.compare(b);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string q, c, n;
    cin >> q >> c >> n;
    
    // Try horizontal: q * (c/n)
    string horizontal = multiplyStrings(q, divideBigInt(c, stoll(n)));
    
    // Try vertical: c * (q/n)
    string vertical = multiplyStrings(c, divideBigInt(q, stoll(n)));
    
    // Output the maximum
    if (compareBigInt(horizontal, vertical) >= 0) {
        cout << horizontal << endl;
    } else {
        cout << vertical << endl;
    }
    
    return 0;
}
