#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
using namespace std;

int compare(string a, string b) {
    int i = 0;
    while (i < a.size() && a[i] == '0') i++;
    a = a.substr(i);
    if (a.empty()) a = "0";
    i = 0;
    while (i < b.size() && b[i] == '0') i++;
    b = b.substr(i);
    if (b.empty()) b = "0";
    
    if (a.size() < b.size()) return -1;
    if (a.size() > b.size()) return 1;
    for (int j = 0; j < a.size(); j++) {
        if (a[j] < b[j]) return -1;
        if (a[j] > b[j]) return 1;
    }
    return 0;
}

string subtract(string a, string b) {
    int i = 0;
    while (i < a.size() && a[i] == '0') i++;
    a = a.substr(i);
    if (a.empty()) a = "0";
    i = 0;
    while (i < b.size() && b[i] == '0') i++;
    b = b.substr(i);
    if (b.empty()) b = "0";
    
    if (compare(a, b) < 0) {
        return "0";
    }
    if (a == b) {
        return "0";
    }
    
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    string res = "";
    int carry = 0;
    for (int idx = 0; idx < a.size(); idx++) {
        int d1 = a[idx] - '0';
        int d2 = (idx < b.size() ? b[idx] - '0' : 0);
        int sub = d1 - d2 - carry;
        if (sub < 0) {
            sub += 10;
            carry = 1;
        } else {
            carry = 0;
        }
        res.push_back(sub + '0');
    }
    reverse(res.begin(), res.end());
    i = 0;
    while (i < res.size() && res[i] == '0') i++;
    if (i == res.size()) return "0";
    return res.substr(i);
}

string multiply(string a, string b) {
    int i = 0;
    while (i < a.size() && a[i] == '0') i++;
    a = a.substr(i);
    if (a.empty()) a = "0";
    i = 0;
    while (i < b.size() && b[i] == '0') i++;
    b = b.substr(i);
    if (b.empty()) b = "0";
    
    if (a == "0" || b == "0") {
        return "0";
    }
    
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    vector<int> res(a.size() + b.size(), 0);
    
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < b.size(); j++) {
            res[i + j] += (a[i] - '0') * (b[j] - '0');
        }
    }
    
    int carry = 0;
    for (int i = 0; i < res.size(); i++) {
        int total = res[i] + carry;
        carry = total / 10;
        res[i] = total % 10;
    }
    while (carry) {
        res.push_back(carry % 10);
        carry /= 10;
    }
    
    string s;
    for (int i = res.size() - 1; i >= 0; i--) {
        s.push_back(res[i] + '0');
    }
    return s;
}

string divide(string a, string b) {
    int i = 0;
    while (i < a.size() && a[i] == '0') i++;
    a = a.substr(i);
    if (a.empty()) a = "0";
    i = 0;
    while (i < b.size() && b[i] == '0') i++;
    b = b.substr(i);
    if (b.empty()) b = "1";
    
    if (a == "0") {
        return "0";
    }
    if (compare(a, b) < 0) {
        return "0";
    }
    
    string quotient = "";
    string current = "";
    for (int idx = 0; idx < a.size(); idx++) {
        current += a[idx];
        i = 0;
        while (i < current.size() && current[i] == '0') i++;
        current = current.substr(i);
        if (current.empty()) current = "0";
        
        int cnt = 0;
        while (compare(current, b) >= 0) {
            current = subtract(current, b);
            cnt++;
        }
        quotient += (char)('0' + cnt);
    }
    
    i = 0;
    while (i < quotient.size() && quotient[i] == '0') i++;
    if (i == quotient.size()) return "0";
    return quotient.substr(i);
}

string add(string a, string b) {
    int i = 0;
    while (i < a.size() && a[i] == '0') i++;
    a = a.substr(i);
    if (a.empty()) a = "0";
    i = 0;
    while (i < b.size() && b[i] == '0') i++;
    b = b.substr(i);
    if (b.empty()) b = "0";
    
    if (a == "0") return b;
    if (b == "0") return a;
    
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    string res = "";
    int carry = 0;
    int n = max(a.size(), b.size());
    for (int i = 0; i < n; i++) {
        int d1 = (i < a.size() ? a[i] - '0' : 0);
        int d2 = (i < b.size() ? b[i] - '0' : 0);
        int sum = d1 + d2 + carry;
        carry = sum / 10;
        res.push_back((sum % 10) + '0');
    }
    if (carry) {
        res.push_back('1');
    }
    reverse(res.begin(), res.end());
    return res;
}

int main() {
    string q_str, c_str, n_str;
    getline(cin, q_str);
    getline(cin, c_str);
    getline(cin, n_str);
    
    auto trim = [](string s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end = s.find_last_not_of(" \t\r\n");
        if (start == string::npos) return string("0");
        return s.substr(start, end - start + 1);
    };
    
    q_str = trim(q_str);
    c_str = trim(c_str);
    n_str = trim(n_str);
    
    if (q_str.empty()) q_str = "0";
    if (c_str.empty()) c_str = "0";
    if (n_str.empty()) n_str = "1";
    
    string A = divide(q_str, n_str);
    string B = divide(c_str, n_str);
    
    string term1 = multiply(q_str, B);
    string term2 = multiply(c_str, A);
    string AB = multiply(A, B);
    string term3 = multiply(n_str, AB);
    
    string total = add(term1, term2);
    string ans = subtract(total, term3);
    cout << ans << endl;
    
    return 0;
}
