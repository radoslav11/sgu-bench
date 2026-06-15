#include<bits/stdc++.h>
using namespace std;

int main(){
    string line;
    getline(cin, line);
    
    // Parse relations
    vector<string> relations;
    size_t pos = line.find("'s");
    pos += 2;
    
    while(pos < line.length()) {
        while(pos < line.length() && !isalpha(line[pos])) pos++;
        if(pos >= line.length()) break;
        
        size_t end = pos;
        while(end < line.length() && isalpha(line[end])) end++;
        
        string word = line.substr(pos, end - pos);
        if(word != "is" && word != "A" && word != "C") {
            relations.push_back(word);
        }
        pos = end;
    }
    
    set<int> distances;
    distances.insert(0);
    
    for(const string& rel : relations) {
        set<int> newDistances;
        
        for(int d : distances) {
            if(rel == "father" || rel == "mother" || rel == "son" || rel == "daughter") {
                newDistances.insert(d + 1);
            }
            else if(rel == "husband" || rel == "wife") {
                newDistances.insert(d);
            }
            else if(rel == "brother" || rel == "sister") {
                newDistances.insert(d + 2);
            }
            else if(rel == "grandfather" || rel == "grandmother") {
                newDistances.insert(d + 2);
            }
            else if(rel == "grandson" || rel == "granddaughter") {
                newDistances.insert(d + 2);
            }
            else if(rel == "uncle" || rel == "aunt") {
                newDistances.insert(d + 2);
            }
            else if(rel == "nephew" || rel == "niece") {
                newDistances.insert(d + 2);
            }
        }
        
        distances = newDistances;
    }
    
    cout << *distances.rbegin() << " " << *distances.begin() << endl;
    
    return 0;
}
