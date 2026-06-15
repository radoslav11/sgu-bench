#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    cin.ignore();
    
    int cells[4] = {0, 0, 0, 0};
    int ptr[4] = {0, 1, 2, 3};
    
    auto idx = [](char c) -> int {
        if(c == 'a') return 0;
        if(c == 'b') return 1;
        if(c == 'c') return 2;
        return 3;
    };
    
    for(int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        if(!line.empty() && line.back() == ';') line.pop_back();
        
        if(line.substr(0, 7) == "writeln") {
            char p = line[8];
            cout << cells[ptr[idx(p)]] << "\n";
        } else {
            size_t pos = line.find(":=");
            string left = line.substr(0, pos);
            string right = line.substr(pos + 2);
            
            if(left.size() == 2 && left[1] == '^') {
                char lp = left[0];
                if(right.size() == 1 && isdigit(right[0])) {
                    cells[ptr[idx(lp)]] = right[0] - '0';
                } else {
                    char rp = right[0];
                    cells[ptr[idx(lp)]] = cells[ptr[idx(rp)]];
                }
            } else {
                char lp = left[0];
                char rp = right[0];
                ptr[idx(lp)] = ptr[idx(rp)];
            }
        }
    }
    
    return 0;
}
