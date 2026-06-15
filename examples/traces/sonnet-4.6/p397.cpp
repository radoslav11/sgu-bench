#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string input;
    cin >> input;
    
    vector<char> left_stack, right_stack;
    left_stack.reserve(input.size());
    right_stack.reserve(input.size());
    
    for(char c : input){
        if(c == 'L'){
            if(!left_stack.empty()){
                right_stack.push_back(left_stack.back());
                left_stack.pop_back();
            }
        } else if(c == 'R'){
            if(!right_stack.empty()){
                left_stack.push_back(right_stack.back());
                right_stack.pop_back();
            }
        } else {
            left_stack.push_back(c);
        }
    }
    
    string result;
    result.reserve(left_stack.size() + right_stack.size());
    for(char c : left_stack) result += c;
    for(int i = right_stack.size()-1; i >= 0; i--) result += right_stack[i];
    
    cout << result << "\n";
    
    return 0;
}
