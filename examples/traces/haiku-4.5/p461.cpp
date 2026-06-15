#include<bits/stdc++.h>
using namespace std;

struct Line {
    int depth;
    char marker;
    string content;
};

vector<Line> parseInput() {
    vector<Line> result;
    string input;
    while(getline(cin, input)) {
        int pos = 0;
        int depth = 0;
        char marker = '\0';
        
        while(pos < input.length() && (input[pos] == '*' || input[pos] == '#')) {
            if(marker == '\0') marker = input[pos];
            if(input[pos] != marker) break;
            depth++;
            pos++;
        }
        
        if(depth == 0) {
            result.push_back({0, '\0', input});
        } else {
            result.push_back({depth, marker, input.substr(pos)});
        }
    }
    return result;
}

int process(vector<Line>& lines, int idx, int maxDepth);

int process(vector<Line>& lines, int idx, int maxDepth) {
    while(idx < lines.size()) {
        if(lines[idx].depth == 0 || lines[idx].depth > maxDepth) {
            break;
        }
        
        if(lines[idx].depth < maxDepth) {
            break;
        }
        
        char marker = lines[idx].marker;
        string tag = (marker == '*') ? "ul" : "ol";
        cout << "<" << tag << ">\n";
        
        while(idx < lines.size() && lines[idx].depth == maxDepth && lines[idx].marker == marker) {
            cout << "<li>\n";
            cout << lines[idx].content << "\n";
            idx++;
            
            if(idx < lines.size() && lines[idx].depth > maxDepth) {
                idx = process(lines, idx, maxDepth + 1);
            }
            
            cout << "</li>\n";
        }
        
        cout << "</" << tag << ">\n";
    }
    return idx;
}

int main(){
    vector<Line> lines = parseInput();
    process(lines, 0, 1);
    return 0;
}
