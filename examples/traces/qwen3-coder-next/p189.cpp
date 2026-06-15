#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <regex>

using namespace std;

// Helper function to trim whitespace from both ends
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Extract variable name from a string like "$var" or "substr(...)"
// Returns the variable name including the '$'
string extractVarName(const string& s) {
    if (!s.empty() && s[0] == '$') {
        size_t end = 1;
        while (end < s.size() && (isalnum(s[end]) || s[end] == '_')) {
            ++end;
        }
        return s.substr(0, end);
    }
    return "";
}

// Parse a substr call: returns {varName, offset, count, hasCount}
// count is optional; if not present, hasCount = false
struct SubstrCall {
    string varName;
    int offset;
    int count;
    bool hasCount;
};

SubstrCall parseSubstr(const string& expr) {
    // expr should be like "substr(var, o[, c])"
    // Remove spaces inside
    string clean;
    for (char c : expr) {
        if (!isspace(c)) clean += c;
    }
    
    // Check format: substr(name,o,c) or substr(name,o)
    if (clean.substr(0, 6) != "substr") {
        // Shouldn't happen per problem, but handle gracefully
        return {"", 0, 0, false};
    }
    
    // Find opening paren
    size_t openParen = clean.find('(');
    size_t closeParen = clean.rfind(')');
    if (openParen == string::npos || closeParen == string::npos) {
        return {"", 0, 0, false};
    }
    
    string inner = clean.substr(openParen + 1, closeParen - openParen - 1);
    
    // Split by commas
    vector<string> parts;
    size_t last = 0;
    for (size_t i = 0; i < inner.size(); ++i) {
        if (inner[i] == ',') {
            parts.push_back(inner.substr(last, i - last));
            last = i + 1;
        }
    }
    parts.push_back(inner.substr(last));
    
    // parts[0] = var name, parts[1] = offset, parts[2] = optional count
    SubstrCall res;
    res.varName = parts[0];
    res.offset = stoi(parts[1]);
    
    if (parts.size() > 2) {
        res.hasCount = true;
        res.count = stoi(parts[2]);
    } else {
        res.hasCount = false;
    }
    
    return res;
}

// Get substring according to Perl rules
string getSubstr(const string& s, int offset, int count, bool hasCount) {
    if (s.empty()) return "";
    
    int n = s.size();
    
    // Convert offset to 0-based from start
    int start;
    if (offset >= 0) {
        start = offset;
    } else {
        start = n + offset; // negative offset: from end
    }
    
    // Clamp start to valid range
    if (start < 0) start = 0;
    if (start > n) start = n;
    
    int length;
    if (hasCount) {
        if (count >= 0) {
            length = count;
        } else {
            // count is negative: leave that many characters off the end
            length = n + count - start;
        }
    } else {
        length = n - start;
    }
    
    // Clamp length to valid range
    if (length < 0) length = 0;
    if (start + length > n) length = n - start;
    
    if (start >= n) return "";
    
    return s.substr(start, length);
}

// Replace substring in string: returns new string
string replaceSubstr(string s, int offset, int count, bool hasCount, const string& replacement) {
    if (s.empty() && offset == 0) {
        // Empty string, offset 0: replace whole string
        return replacement;
    }
    
    int n = s.size();
    
    int start;
    if (offset >= 0) {
        start = offset;
    } else {
        start = n + offset;
    }
    
    // Clamp start to valid range
    if (start < 0) start = 0;
    if (start > n) start = n;
    
    int length;
    if (hasCount) {
        if (count >= 0) {
            length = count;
        } else {
            length = n + count - start;
        }
    } else {
        length = n - start;
    }
    
    // Clamp length to valid range
    if (length < 0) length = 0;
    if (start + length > n) length = n - start;
    
    // Perform replacement
    return s.substr(0, start) + replacement + s.substr(start + length);
}

// Parse a line to determine its type and extract relevant info
// Return type and parsed data
struct LineInfo {
    int type; // 1: print(var), 2: print(substr(...)), 3: var1=var2, 
              // 4: var1=substr(...), 5: substr(...)=var2, 6: substr(...)=substr(...)
    string destVar;
    string srcVar1;
    string srcVar2;
    int offset1, count1;
    bool hasCount1;
    int offset2, count2;
    bool hasCount2;
    bool isDestSubstr;
    bool isSrcSubstr;
};

LineInfo parseLine(const string& line) {
    LineInfo info;
    info.type = 0;
    info.isDestSubstr = false;
    info.isSrcSubstr = false;
    
    string cleanLine = line;
    // Remove semicolon and trailing whitespace
    if (!cleanLine.empty() && cleanLine.back() == ';') {
        cleanLine.pop_back();
    }
    
    // Find '=' sign
    size_t eqPos = cleanLine.find("=");
    if (eqPos == string::npos) {
        // It's a print statement
        // Extract content inside print(...)
        size_t printStart = cleanLine.find("print(");
        if (printStart == 0) {
            size_t openParen = cleanLine.find('(');
            size_t closeParen = cleanLine.rfind(')');
            if (openParen != string::npos && closeParen != string::npos) {
                string inner = cleanLine.substr(openParen + 1, closeParen - openParen - 1);
                if (inner.substr(0, 6) == "substr") {
                    info.type = 2;
                    SubstrCall sc = parseSubstr(inner);
                    info.srcVar1 = sc.varName;
                    info.offset1 = sc.offset;
                    info.count1 = sc.count;
                    info.hasCount1 = sc.hasCount;
                } else {
                    // It's a variable name
                    info.type = 1;
                    info.srcVar1 = inner;
                }
            }
        }
        return info;
    }
    
    string left = cleanLine.substr(0, eqPos);
    string right = cleanLine.substr(eqPos + 1);
    
    left = trim(left);
    right = trim(right);
    
    // Check if left is a substr call
    if (left.substr(0, 6) == "substr") {
        info.isDestSubstr = true;
        SubstrCall sc = parseSubstr(left);
        info.srcVar1 = sc.varName;
        info.offset1 = sc.offset;
        info.count1 = sc.count;
        info.hasCount1 = sc.hasCount;
    } else {
        info.destVar = left;
    }
    
    // Check if right is a substr call
    if (right.substr(0, 6) == "substr") {
        info.isSrcSubstr = true;
        SubstrCall sc = parseSubstr(right);
        info.srcVar2 = sc.varName;
        info.offset2 = sc.offset;
        info.count2 = sc.count;
        info.hasCount2 = sc.hasCount;
    } else {
        info.srcVar2 = right;
    }
    
    // Determine type
    if (info.isDestSubstr && info.isSrcSubstr) {
        info.type = 6;
    } else if (info.isDestSubstr && !info.isSrcSubstr) {
        info.type = 5;
    } else if (!info.isDestSubstr && info.isSrcSubstr) {
        info.type = 4;
    } else if (!info.isDestSubstr && !info.isSrcSubstr) {
        info.type = 3;
    }
    
    return info;
}

int main() {
    int N, M;
    cin >> N >> M;
    cin.ignore(); // consume newline
    
    map<string, string> vars; // variable name to value
    
    // Read N initialization lines
    for (int i = 0; i < N; ++i) {
        string line;
        getline(cin, line);
        
        // Parse: name = "value";
        size_t eqPos = line.find("=");
        if (eqPos == string::npos) continue;
        
        string varName = trim(line.substr(0, eqPos));
        string valuePart = trim(line.substr(eqPos + 1));
        
        // Remove semicolon at the end
        if (!valuePart.empty() && valuePart.back() == ';') {
            valuePart.pop_back();
        }
        valuePart = trim(valuePart);
        
        // Extract string between quotes
        if (valuePart.size() >= 2 && valuePart.front() == '"' && valuePart.back() == '"') {
            string actualValue = valuePart.substr(1, valuePart.size() - 2);
            vars[varName] = actualValue;
        }
    }
    
    // Read M program lines
    for (int i = 0; i < M; ++i) {
        string line;
        getline(cin, line);
        
        LineInfo info = parseLine(line);
        
        switch (info.type) {
            case 1: { // print(var)
                string varName = info.srcVar1;
                string value = vars.count(varName) ? vars[varName] : "";
                cout << value << "\n";
                break;
            }
            case 2: { // print(substr(...))
                string varName = info.srcVar1;
                string s = vars.count(varName) ? vars[varName] : "";
                string sub = getSubstr(s, info.offset1, info.count1, info.hasCount1);
                cout << sub << "\n";
                break;
            }
            case 3: { // var1 = var2
                string var1 = info.destVar;
                string var2 = info.srcVar2;
                string val = vars.count(var2) ? vars[var2] : "";
                vars[var1] = val;
                break;
            }
            case 4: { // var1 = substr(var2, o[, c])
                string var1 = info.destVar;
                string var2 = info.srcVar2;
                string s2 = vars.count(var2) ? vars[var2] : "";
                string sub = getSubstr(s2, info.offset1, info.count1, info.hasCount1);
                vars[var1] = sub;
                break;
            }
            case 5: { // substr(var1, o[, c]) = var2
                string var1 = info.srcVar1;
                string var2 = info.srcVar2;
                string s1 = vars.count(var1) ? vars[var1] : "";
                string s2 = vars.count(var2) ? vars[var2] : "";
                string newS1 = replaceSubstr(s1, info.offset1, info.count1, info.hasCount1, s2);
                vars[var1] = newS1;
                break;
            }
            case 6: { // substr(var1, o[, c]) = substr(var2, o[, c])
                string var1 = info.srcVar1;
                string var2 = info.srcVar2;
                string s1 = vars.count(var1) ? vars[var1] : "";
                string s2 = vars.count(var2) ? vars[var2] : "";
                string sub2 = getSubstr(s2, info.offset2, info.count2, info.hasCount2);
                string newS1 = replaceSubstr(s1, info.offset1, info.count1, info.hasCount1, sub2);
                vars[var1] = newS1;
                break;
            }
        }
    }
    
    return 0;
}
