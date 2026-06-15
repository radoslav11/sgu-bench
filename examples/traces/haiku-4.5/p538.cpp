#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    getline(cin, s);
    
    int count = 0;
    int n = s.length();
    
    // Check each opening bracket
    for(int i = 0; i < n; i++){
        if(s[i] == '('){
            // Look for nearest closing bracket to the right
            bool is_emoticon = true;
            int j = i + 1;
            while(j < n){
                if(s[j] == ')'){
                    // Found closing bracket, check if only letters/spaces between
                    bool valid = true;
                    for(int k = i + 1; k < j; k++){
                        if(s[k] != ' ' && !isalpha(s[k])){
                            valid = false;
                            break;
                        }
                    }
                    if(valid){
                        is_emoticon = false;
                    }
                    break;
                }
                j++;
            }
            if(is_emoticon){
                count++;
            }
        }
    }
    
    // Check each closing bracket
    for(int i = 0; i < n; i++){
        if(s[i] == ')'){
            // Look for nearest opening bracket to the left
            bool is_emoticon = true;
            int j = i - 1;
            while(j >= 0){
                if(s[j] == '('){
                    // Found opening bracket, check if only letters/spaces between
                    bool valid = true;
                    for(int k = j + 1; k < i; k++){
                        if(s[k] != ' ' && !isalpha(s[k])){
                            valid = false;
                            break;
                        }
                    }
                    if(valid){
                        is_emoticon = false;
                    }
                    break;
                }
                j--;
            }
            if(is_emoticon){
                count++;
            }
        }
    }
    
    cout << count << endl;
    
    return 0;
}
