#include <iostream>
#include <vector>
#include <string>

using namespace std;

string process(vector<string> lines) {
    if (lines.empty()) return "";
    if (lines.size() == 1) return lines[0];
    
    char marker = 0;
    bool all_valid = true;
    for (const string& line : lines) {
        if (line.empty() || (line[0] != '*' && line[0] != '#')) {
            all_valid = false;
            break;
        }
        if (marker == 0) marker = line[0];
        else if (marker != line[0]) {
            all_valid = false;
            break;
        }
    }
    
    if (all_valid) {
        vector<string> stripped;
        for (const string& line : lines) {
            stripped.push_back(line.substr(1));
        }
        
        string result = "";
        if (marker == '*') result += "<ul>\n";
        else result += "<ol>\n";
        
        vector<string> current_blk;
        char cur_m = 0;
        
        auto flush_blk = [&]() {
            if (current_blk.empty()) return;
            string blk_res = process(current_blk);
            result += "<li>\n" + blk_res + "\n</li>\n";
            current_blk.clear();
        };
        
        for (const string& line : stripped) {
            char m = (line.empty() || (line[0] != '*' && line[0] != '#')) ? 0 : line[0];
            if (m != cur_m || m == 0) {
                flush_blk();
                cur_m = m;
            }
            current_blk.push_back(line);
        }
        flush_blk();
        
        if (marker == '*') result += "</ul>\n";
        else result += "</ol>\n";
        
        return result;
    } else {
        vector<string> parts;
        vector<string> current_blk;
        char cur_m = 0;
        
        auto flush_blk2 = [&]() {
            if (current_blk.empty()) return;
            parts.push_back(process(current_blk));
            current_blk.clear();
        };
        
        for (const string& line : lines) {
            char m = (line.empty() || (line[0] != '*' && line[0] != '#')) ? 0 : line[0];
            if (m != cur_m || m == 0) {
                flush_blk2();
                cur_m = m;
            }
            current_blk.push_back(line);
        }
        flush_blk2();
        
        string result = "";
        for (size_t i = 0; i < parts.size(); ++i) {
            result += parts[i] + (i + 1 == parts.size() ? "" : "\n");
        }
        return result;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    vector<string> lines;
    string line;
    while (getline(cin, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    
    if (!lines.empty()) {
        cout << process(lines);
    }
    
    return 0;
}
