#include <iostream>
#include <string>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (cin >> n) {
        while (n--) {
            string s;
            cin >> s;
            int len = s.length();
            
            if (len >= 2 && s.substr(len - 2) == "ch") {
                cout << s << "es\n";
            } else if (s.back() == 'x' || s.back() == 's' || s.back() == 'o') {
                cout << s << "es\n";
            } else if (len >= 2 && s.substr(len - 2) == "fe") {
                cout << s.substr(0, len - 2) << "ves\n";
            } else if (s.back() == 'f') {
                cout << s.substr(0, len - 1) << "ves\n";
            } else if (s.back() == 'y') {
                cout << s.substr(0, len - 1) << "ies\n";
            } else {
                cout << s << "s\n";
            }
        }
    }
    
    return 0;
}
