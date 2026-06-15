#include <bits/stdc++.h>
using namespace std;

int main(){
    string s;
    getline(cin, s);
    int n = s.size();
    auto isBracket = [&](char c){ return c=='('||c==')'; };
    auto isLetterSpace = [&](char c){ return c==' '||(c>='a'&&c<='z')||(c>='A'&&c<='Z'); };
    
    int total = 0, real = 0;
    for(int i=0;i<n;i++){
        if(s[i]=='('){
            total++;
            // nearest bracket to right
            int j=i+1; bool ok=true;
            while(j<n && !isBracket(s[j])){
                if(!isLetterSpace(s[j])){ ok=false; break; }
                j++;
            }
            if(ok && j<n && s[j]==')') real++;
        } else if(s[i]==')'){
            total++;
            int j=i-1; bool ok=true;
            while(j>=0 && !isBracket(s[j])){
                if(!isLetterSpace(s[j])){ ok=false; break; }
                j--;
            }
            if(ok && j>=0 && s[j]=='(') real++;
        }
    }
    cout << (total-real) << endl;
    return 0;
}
