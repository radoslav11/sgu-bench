#include<bits/stdc++.h>
using namespace std;

int main(){
    string input;
    getline(cin, input);
    
    vector<string> lines;
    lines.push_back("");
    
    int row = 0, col = 0;
    
    for(char cmd : input){
        if(cmd == 'Q') break;
        
        // Ensure we have enough lines
        while(row >= lines.size()) {
            lines.push_back("");
        }
        
        if(cmd == 'L'){
            if(col > 0) col--;
        }
        else if(cmd == 'R'){
            col++;
        }
        else if(cmd == 'U'){
            if(row > 0) row--;
        }
        else if(cmd == 'D'){
            if(row < lines.size() - 1) row++;
        }
        else if(cmd == 'H'){
            col = 0;
        }
        else if(cmd == 'E'){
            col = lines[row].length();
        }
        else if(cmd == 'N'){
            string newLine = lines[row].substr(col);
            lines[row] = lines[row].substr(0, col);
            lines.insert(lines.begin() + row + 1, newLine);
            row++;
            col = 0;
        }
        else if(cmd == 'X'){
            if(col < lines[row].length()){
                lines[row].erase(col, 1);
            }
            else if(col >= lines[row].length() && row < lines.size() - 1){
                // Add spaces and attach next line
                lines[row] += string(col - lines[row].length(), ' ');
                lines[row] += lines[row + 1];
                lines.erase(lines.begin() + row + 1);
            }
        }
        else if(cmd == 'B'){
            if(col > 0){
                lines[row].erase(col - 1, 1);
                col--;
            }
            else if(col == 0 && row > 0){
                // Attach current line to previous
                col = lines[row - 1].length();
                lines[row - 1] += lines[row];
                lines.erase(lines.begin() + row);
                row--;
            }
            else if(col >= lines[row].length() && col > 0){
                col--;
            }
        }
        else {
            // Insert character (letter or space)
            if(col > lines[row].length()){
                lines[row] += string(col - lines[row].length(), ' ');
            }
            lines[row].insert(col, 1, cmd);
            col++;
        }
    }
    
    // Output result
    for(int i = 0; i < lines.size(); i++){
        cout << lines[i];
        if(i < lines.size() - 1) cout << "\n";
    }
    
    return 0;
}
