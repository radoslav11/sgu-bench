#include <bits/stdc++.h>
using namespace std;

int main(){
    vector<string> lines;
    lines.push_back("");
    int row = 0, col = 0;
    
    string input;
    {
        char c;
        while(cin.get(c)){
            if(c=='\n' || c=='\r') continue;
            input.push_back(c);
        }
    }
    
    for(size_t i=0;i<input.size();i++){
        char c = input[i];
        if((c>='a'&&c<='z') || c==' '){
            string &L = lines[row];
            while((int)L.size() < col) L.push_back(' ');
            L.insert(L.begin()+col, c);
            col++;
        } else if(c=='L'){
            if(col>0) col--;
        } else if(c=='R'){
            col++;
        } else if(c=='U'){
            if(row>0) row--;
        } else if(c=='D'){
            if(row < (int)lines.size()-1) row++;
        } else if(c=='N'){
            string &L = lines[row];
            string tail;
            if(col < (int)L.size()){
                tail = L.substr(col);
                L = L.substr(0,col);
            }
            lines.insert(lines.begin()+row+1, tail);
            row++;
            col=0;
        } else if(c=='E'){
            col = lines[row].size();
        } else if(c=='H'){
            col = 0;
        } else if(c=='X'){
            string &L = lines[row];
            if(col < (int)L.size()){
                L.erase(L.begin()+col);
            } else {
                if(row < (int)lines.size()-1){
                    while((int)L.size() < col) L.push_back(' ');
                    L += lines[row+1];
                    lines.erase(lines.begin()+row+1);
                }
            }
        } else if(c=='B'){
            string &L = lines[row];
            if(col > (int)L.size()){
                col--;
            } else if(col > 0){
                L.erase(L.begin()+(col-1));
                col--;
            } else {
                if(row>0){
                    int prevLen = lines[row-1].size();
                    lines[row-1] += L;
                    lines.erase(lines.begin()+row);
                    row--;
                    col = prevLen;
                }
            }
        } else if(c=='Q'){
            break;
        }
    }
    
    for(size_t i=0;i<lines.size();i++){
        if(i>0) cout << (char)13 << (char)10;
        cout << lines[i];
    }
    
    return 0;
}
