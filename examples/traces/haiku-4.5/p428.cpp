#include <bits/stdc++.h>
using std::cout, std::string;

int solutionCount(string a, string b, string c) {
    int n = a.length();
    set<char> chars;
    for(char ch : a) chars.insert(ch);
    for(char ch : b) chars.insert(ch);
    for(char ch : c) chars.insert(ch);
    
    if(chars.size() > 10) return 0;
    
    vector<char> charList(chars.begin(), chars.end());
    vector<int> perm(10);
    iota(perm.begin(), perm.end(), 0);
    
    int count = 0;
    do {
        std::map<char, int> mp;
        for(int i = 0; i < charList.size(); i++) mp[charList[i]] = perm[i];
        
        if(mp[a[0]] == 0 || mp[b[0]] == 0 || mp[c[0]] == 0) continue;
        
        long long numA = 0, numB = 0, numC = 0;
        for(char ch : a) numA = numA * 10 + mp[ch];
        for(char ch : b) numB = numB * 10 + mp[ch];
        for(char ch : c) numC = numC * 10 + mp[ch];
        
        if(numA + numB == numC) count++;
    } while(next_permutation(perm.begin(), perm.end()) && count <= 1);
    
    return count;
}

int main(){
    int found = 0;
    for(int len = 2; len <= 3 && found < 1000; len++) {
        for(int a = 0; a < 26 && found < 1000; a++) {
            for(int b = 0; b < 26 && found < 1000; b++) {
                if(a == b) continue;
                for(int c = 0; c < 26 && found < 1000; c++) {
                    if(c == a || c == b) continue;
                    string sa(len, 'A' + a);
                    string sb(len, 'A' + b);
                    string sc(len, 'A' + c);
                    if(solutionCount(sa, sb, sc) == 1) {
                        cout << sa << "+" << sb << "=" << sc << "\n";
                        found++;
                    }
                }
            }
        }
    }
    return 0;
}
