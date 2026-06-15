#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    while(n--) {
        string word;
        cin >> word;
        
        string plural;
        int len = word.length();
        
        // Rule 2: ends with "fe" or "f"
        if(len >= 2 && word.substr(len-2) == "fe") {
            plural = word.substr(0, len-2) + "ves";
        }
        else if(len >= 1 && word[len-1] == 'f') {
            plural = word.substr(0, len-1) + "ves";
        }
        // Rule 3: ends with "y"
        else if(len >= 1 && word[len-1] == 'y') {
            plural = word.substr(0, len-1) + "ies";
        }
        // Rule 1: ends with "ch", "x", "s", "o"
        else if((len >= 2 && word.substr(len-2) == "ch") ||
                (len >= 1 && (word[len-1] == 'x' || word[len-1] == 's' || word[len-1] == 'o'))) {
            plural = word + "es";
        }
        // Rule 4: default
        else {
            plural = word + "s";
        }
        
        cout << plural << "\n";
    }
    
    return 0;
}
