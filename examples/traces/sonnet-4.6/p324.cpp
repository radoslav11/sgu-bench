#include <bits/stdc++.h>
using namespace std;
int main() {
    int n; cin >> n; cin.ignore();
    while(n--) {
        string line; getline(cin, line);
        string text = line.substr(1, line.size()-2);
        vector<pair<int,string>> tokens;
        int i = 0;
        while(i < (int)text.size()) {
            if(isalpha(text[i])) {
                string w; while(i<(int)text.size()&&isalpha(text[i])) w+=text[i++];
                tokens.push_back({0,w});
            } else if(text[i]=='-') {
                string d; while(i<(int)text.size()&&text[i]=='-') d+=text[i++];
                tokens.push_back({1,d});
            } else i++;
        }
        bool err = false;
        string out = "";
        for(int j = 0; j < (int)tokens.size(); j++) {
            auto [type, val] = tokens[j];
            if(type == 1) {
                int len = val.size();
                if(len == 1) {
                    if(j==0||j==(int)tokens.size()-1||tokens[j-1].first!=0||tokens[j+1].first!=0) { err=true; break; }
                    out += "-";
                } else if(len==2||len==3) {
                    if(!out.empty()&&out.back()!=' ') out+=" ";
                    out += val;
                    if(j+1<(int)tokens.size()) out+=" ";
                } else { err=true; break; }
            } else {
                if(j>0&&tokens[j-1].first==1&&(int)tokens[j-1].second.size()==1) {
                    out += val;
                } else {
                    if(!out.empty()&&out.back()!=' ') out+=" ";
                    out += val;
                }
            }
        }
        if(err) cout << "error" << "\n";
        else cout << "\"" << out << "\"" << "\n";
    }
}
