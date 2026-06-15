#include<bits/stdc++.h>
using namespace std;

map<string, int> values;
map<string, int> dff_state;
map<string, pair<string, vector<string>>> gates; // gate_name -> (gate_type, inputs)
vector<string> inputs;
vector<string> outputs;
set<string> computed_this_tick;

int evaluate(const string& junction);

int evaluate(const string& junction) {
    if(computed_this_tick.count(junction)) {
        return values[junction];
    }
    
    if(gates.count(junction) == 0) {
        // It's an input or already computed
        return values[junction];
    }
    
    auto& [op, inputs_list] = gates[junction];
    computed_this_tick.insert(junction);
    
    if(op == "NOT") {
        int val = evaluate(inputs_list[0]);
        values[junction] = 1 - val;
    }
    else if(op == "AND") {
        int result = 1;
        for(auto& inp : inputs_list) {
            result &= evaluate(inp);
        }
        values[junction] = result;
    }
    else if(op == "NAND") {
        int result = 1;
        for(auto& inp : inputs_list) {
            result &= evaluate(inp);
        }
        values[junction] = 1 - result;
    }
    else if(op == "OR") {
        int result = 0;
        for(auto& inp : inputs_list) {
            result |= evaluate(inp);
        }
        values[junction] = result;
    }
    else if(op == "NOR") {
        int result = 0;
        for(auto& inp : inputs_list) {
            result |= evaluate(inp);
        }
        values[junction] = 1 - result;
    }
    else if(op == "DFF") {
        values[junction] = dff_state[junction];
        dff_state[junction] = evaluate(inputs_list[0]);
    }
    
    return values[junction];
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string line;
    
    // Parse circuit
    while(getline(cin, line)) {
        if(line.empty() || line[0] == '#') continue;
        if(line.find("INPUT VALUES") != string::npos) break;
        
        if(line.find("INPUT(") == 0) {
            int start = 6;
            int end = line.find(')', start);
            inputs.push_back(line.substr(start, end - start));
        }
        else if(line.find("OUTPUT(") == 0) {
            int start = 7;
            int end = line.find(')', start);
            outputs.push_back(line.substr(start, end - start));
        }
        else {
            // Parse gate definition: j1 = op(j2, j3, ...)
            int eq_pos = line.find('=');
            string j1 = line.substr(0, eq_pos);
            j1.erase(j1.find_last_not_of(" ") + 1);
            
            int op_start = eq_pos + 1;
            while(op_start < line.length() && line[op_start] == ' ') op_start++;
            
            int paren_pos = line.find('(', op_start);
            string op = line.substr(op_start, paren_pos - op_start);
            op.erase(op.find_last_not_of(" ") + 1);
            
            vector<string> inp_list;
            int start = paren_pos + 1;
            for(int i = paren_pos + 1; i <= line.length(); i++) {
                if(i == line.length() || line[i] == ',' || line[i] == ')') {
                    if(i > start) {
                        string inp = line.substr(start, i - start);
                        inp.erase(0, inp.find_first_not_of(" "));
                        inp.erase(inp.find_last_not_of(" ") + 1);
                        inp_list.push_back(inp);
                    }
                    start = i + 1;
                }
            }
            
            gates[j1] = {op, inp_list};
            dff_state[j1] = 0;
        }
    }
    
    // Process input values
    while(getline(cin, line)) {
        if(line.empty()) continue;
        
        computed_this_tick.clear();
        
        for(int i = 0; i < inputs.size(); i++) {
            values[inputs[i]] = line[i] - '0';
        }
        
        string result = "";
        for(auto& out : outputs) {
            result += (char)('0' + evaluate(out));
        }
        cout << result << "\n";
    }
    
    return 0;
}
