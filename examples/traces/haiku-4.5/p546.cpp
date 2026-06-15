#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, a, b;
    string password;
    cin >> n >> a >> b >> password;
    
    // Check if solution is possible
    if(a + b > n) {
        cout << -1 << endl;
        return 0;
    }
    
    int c = n - a - b; // expected number of 2s
    
    // Count current occurrences
    int cnt0 = 0, cnt1 = 0, cnt2 = 0;
    for(char ch : password) {
        if(ch == '0') cnt0++;
        else if(ch == '1') cnt1++;
        else cnt2++;
    }
    
    int changes = 0;
    
    // First pass: replace characters we have too many of
    // Replace excess 0s
    for(int i = 0; i < n && cnt0 > a; i++) {
        if(password[i] == '0') {
            if(cnt1 < b) {
                password[i] = '1';
                cnt0--;
                cnt1++;
                changes++;
            } else if(cnt2 < c) {
                password[i] = '2';
                cnt0--;
                cnt2++;
                changes++;
            }
        }
    }
    
    // Replace excess 1s
    for(int i = 0; i < n && cnt1 > b; i++) {
        if(password[i] == '1') {
            if(cnt0 < a) {
                password[i] = '0';
                cnt1--;
                cnt0++;
                changes++;
            } else if(cnt2 < c) {
                password[i] = '2';
                cnt1--;
                cnt2++;
                changes++;
            }
        }
    }
    
    // Replace excess 2s
    for(int i = 0; i < n && cnt2 > c; i++) {
        if(password[i] == '2') {
            if(cnt0 < a) {
                password[i] = '0';
                cnt2--;
                cnt0++;
                changes++;
            } else if(cnt1 < b) {
                password[i] = '1';
                cnt2--;
                cnt1++;
                changes++;
            }
        }
    }
    
    cout << changes << endl;
    cout << password << endl;
    
    return 0;
}
