#include <bits/stdc++.h>
using namespace std;
int main(){
    string input;
    getline(cin, input);
    vector<string> lines;
    lines.push_back("");
    int row=0,col=0;
    for(char c:input){
        if(c=='Q') break;
        else if(c=='L'){if(col>0)col--;}
        else if(c=='R'){col++;}
        else if(c=='U'){if(row>0)row--;}
        else if(c=='D'){if(row<(int)lines.size()-1)row++;}
        else if(c=='H'){col=0;}
        else if(c=='E'){col=lines[row].size();}
        else if(c=='N'){
            string cur=lines[row];
            string before=(col<=(int)cur.size())?cur.substr(0,col):cur;
            string after=(col<=(int)cur.size())?cur.substr(col):"";
            lines[row]=before;
            lines.insert(lines.begin()+row+1,after);
            row++;col=0;
        }
        else if(c=='X'){
            string&cur=lines[row];
            if(col<(int)cur.size()){cur.erase(col,1);}
            else{
                if(row==(int)lines.size()-1){}
                else{while((int)cur.size()<col)cur+=' ';cur+=lines[row+1];lines.erase(lines.begin()+row+1);}
            }
        }
        else if(c=='B'){
            string&cur=lines[row];
            if(col>(int)cur.size()){col--;}
            else if(col==0){
                if(row==0){}
                else{int nc=lines[row-1].size();lines[row-1]+=cur;lines.erase(lines.begin()+row);row--;col=nc;}
            }
            else{cur.erase(col-1,1);col--;}
        }
        else{
            string&cur=lines[row];
            while((int)cur.size()<col)cur+=' ';
            cur.insert(col,1,c);col++;
        }
    }
    for(int i=0;i<(int)lines.size();i++){if(i>0)cout<<"\n";cout<<lines[i];}
    return 0;
}
