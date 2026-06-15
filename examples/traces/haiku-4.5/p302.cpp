#include <bits/stdc++.h>
using namespace std;

int main(){
    string s;
    getline(cin, s);
    
    stack<string> tags;
    string result = "";
    
    for(int i = 0; i < s.length(); i++){
        if(s[i] == '<'){
            // Find the end of tag
            int j = i;
            while(j < s.length() && s[j] != '>') j++;
            string tag = s.substr(i+1, j-i-1);
            
            if(tag == "UP"){
                tags.push("UP");
            } else if(tag == "/UP"){
                tags.pop();
            } else if(tag == "DOWN"){
                tags.push("DOWN");
            } else if(tag == "/DOWN"){
                tags.pop();
            }
            
            i = j;
        } else if(isalpha(s[i])){
            char c = s[i];
            if(!tags.empty()){
                if(tags.top() == "UP"){
                    c = toupper(c);
                } else {
                    c = tolower(c);
                }
            }
            result += c;
        } else {
            result += s[i];
        }
    }
    
    cout << result << endl;
    
    return 0;
}
