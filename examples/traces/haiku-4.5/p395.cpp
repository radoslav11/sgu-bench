#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    cin.ignore();
    
    vector<string> records;
    vector<pair<char, string>> parsed; // '+'/'-'/'=' and name/count
    
    for(int i = 0; i < n; i++){
        string line;
        getline(cin, line);
        records.push_back(line);
        
        if(line[0] == '+' || line[0] == '-'){
            parsed.push_back({line[0], line.substr(2)});
        } else {
            parsed.push_back({'=', line.substr(2)});
        }
    }
    
    vector<string> result;
    set<string> inside;
    
    for(auto& p : parsed){
        char op = p.first;
        string arg = p.second;
        
        if(op == '+'){
            // Person entering - must not be inside
            if(inside.count(arg)){
                // Need to remove first
                result.push_back("- " + arg);
                inside.erase(arg);
            }
            result.push_back("+ " + arg);
            inside.insert(arg);
        }
        else if(op == '-'){
            // Person leaving - must be inside
            if(!inside.count(arg)){
                // Need to add first
                result.push_back("+ " + arg);
                inside.insert(arg);
            }
            result.push_back("- " + arg);
            inside.erase(arg);
        }
        else { // op == '='
            int needed = stoi(arg);
            int current = inside.size();
            
            if(current < needed){
                // Need to add people
                for(int i = 0; i < needed - current; i++){
                    string name = "guest" + to_string(i);
                    while(inside.count(name)){
                        name = "guest" + to_string(++i);
                    }
                    result.push_back("+ " + name);
                    inside.insert(name);
                }
            }
            else if(current > needed){
                // Need to remove people
                vector<string> to_remove;
                for(auto& name : inside){
                    to_remove.push_back(name);
                    if(to_remove.size() == current - needed) break;
                }
                for(auto& name : to_remove){
                    result.push_back("- " + name);
                    inside.erase(name);
                }
            }
            result.push_back("= " + arg);
        }
    }
    
    cout << result.size() << "\n";
    for(auto& s : result){
        cout << s << "\n";
    }
    
    return 0;
}
