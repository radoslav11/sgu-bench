#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    cin >> s;
    int n = s.length();
    
    // Try to find rightmost position where we can change ')' to '('
    for(int i = n - 1; i >= 0; i--){
        if(s[i] == ')'){
            // Try changing this ')' to '('
            string temp = s.substr(0, i) + '(' + string(n - i - 1, ' ');
            
            // Check if prefix is valid
            int balance = 0;
            bool valid = true;
            for(int j = 0; j <= i; j++){
                if(temp[j] == '(') balance++;
                else balance--;
                if(balance < 0){
                    valid = false;
                    break;
                }
            }
            
            if(!valid) continue;
            
            // Count remaining '(' and ')' needed
            int remaining_len = n - i - 1;
            int open_needed = 0, close_needed = 0;
            
            // Current balance after position i
            for(int j = 0; j <= i; j++){
                if(temp[j] == '(') open_needed++;
                else open_needed--;
            }
            
            // We need to add brackets such that:
            // - Total '(' in suffix = close_needed
            // - Total ')' in suffix = open_needed (to close all opens)
            // - remaining_len = close_needed + open_needed
            
            close_needed = (remaining_len - open_needed) / 2;
            open_needed = remaining_len - close_needed;
            
            if(open_needed < 0 || close_needed < 0) continue;
            if(open_needed + close_needed != remaining_len) continue;
            
            // Fill suffix: all '(' first, then all ')'
            for(int j = 0; j < open_needed; j++){
                temp[i + 1 + j] = '(';
            }
            for(int j = 0; j < close_needed; j++){
                temp[i + 1 + open_needed + j] = ')';
            }
            
            // Verify the entire string is valid
            balance = 0;
            valid = true;
            for(int j = 0; j < n; j++){
                if(temp[j] == '(') balance++;
                else balance--;
                if(balance < 0){
                    valid = false;
                    break;
                }
            }
            
            if(valid && balance == 0){
                cout << temp << endl;
                return 0;
            }
        }
    }
    
    cout << "No solution" << endl;
    return 0;
}
