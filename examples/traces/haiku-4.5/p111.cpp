#include<bits/stdc++.h>
using namespace std;

string multiply(string a, string b) {
    int n = a.length(), m = b.length();
    vector<int> result(n + m, 0);
    
    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            int mul = (a[i] - '0') * (b[j] - '0');
            int p1 = i + j, p2 = i + j + 1;
            int sum = mul + result[p2];
            result[p2] = sum % 10;
            result[p1] += sum / 10;
        }
    }
    
    string str = "";
    for (int num : result) {
        if (!(str.empty() && num == 0)) str += char('0' + num);
    }
    return str.empty() ? "0" : str;
}

int main(){
    string X;
    cin >> X;
    
    string low = "1", high = X, ans = "1";
    
    while (low.length() < high.length() || (low.length() == high.length() && low <= high)) {
        string mid = low;
        string sq = multiply(mid, mid);
        
        if (sq.length() < X.length() || (sq.length() == X.length() && sq <= X)) {
            ans = mid;
            int i = mid.length() - 1, carry = 1;
            while (i >= 0 && carry) {
                int d = mid[i] - '0' + 1;
                mid[i] = '0' + d % 10;
                carry = d / 10;
                i--;
            }
            if (carry) mid = "1" + mid;
            low = mid;
        } else {
            break;
        }
    }
    
    cout << ans << endl;
    return 0;
}
