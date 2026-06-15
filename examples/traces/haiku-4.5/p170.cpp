#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s1, s2;
    cin >> s1 >> s2;
    
    if(s1.length() != s2.length()){
        cout << -1 << endl;
        return 0;
    }
    
    int count1_plus = 0, count2_plus = 0;
    for(char c : s1) if(c == '+') count1_plus++;
    for(char c : s2) if(c == '+') count2_plus++;
    
    if(count1_plus != count2_plus){
        cout << -1 << endl;
        return 0;
    }
    
    string current = s1;
    int swaps = 0;
    
    for(int i = 0; i < (int)s2.length(); i++){
        if(current[i] == s2[i]) continue;
        
        int j = i + 1;
        while(j < (int)current.length() && current[j] != s2[i]){
            j++;
        }
        
        while(j > i){
            swap(current[j], current[j-1]);
            j--;
            swaps++;
        }
    }
    
    cout << swaps << endl;
    return 0;
}
