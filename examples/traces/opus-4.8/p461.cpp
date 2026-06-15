#include <bits/stdc++.h>
using namespace std;

void process(vector<string>& lines){
    int n = lines.size(), i = 0;
    while(i < n){
        char c = lines[i].empty() ? 0 : lines[i][0];
        if(c=='#' || c=='*'){
            int j = i;
            while(j<n && !lines[j].empty() && lines[j][0]==c) j++;
            if(j-i >= 2){
                cout << (c=='#' ? "<ol>" : "<ul>") << "\n";
                vector<string> sub;
                for(int k=i;k<j;k++) sub.push_back(lines[k].substr(1));
                int m = sub.size(), p = 0;
                while(p<m){
                    char cc = sub[p].empty()?0:sub[p][0];
                    if(cc=='#'||cc=='*'){
                        int q=p;
                        while(q<m && !sub[q].empty() && sub[q][0]==cc) q++;
                        if(q-p>=2){
                            cout << "<li>\n";
                            vector<string> seg(sub.begin()+p, sub.begin()+q);
                            process(seg);
                            cout << "</li>\n";
                            p=q; continue;
                        }
                    }
                    cout << "<li>\n" << sub[p] << "\n" << "</li>\n";
                    p++;
                }
                cout << (c=='#' ? "</ol>" : "</ul>") << "\n";
                i = j; continue;
            }
        }
        cout << lines[i] << "\n";
        i++;
    }
}

int main(){
    vector<string> lines; string s;
    while(getline(cin,s)) lines.push_back(s);
    process(lines);
}
