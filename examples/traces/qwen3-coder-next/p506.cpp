#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    string text, message;
    getline(cin, text);
    getline(cin, message);
    
    int n = text.length();
    int m = message.length();
    
    long long result = 0;
    
    for (int l = 0; l < n; l++) {
        int j = 0;
        int r = -1;
        for (int i = l; i < n; i++) {
            if (j < m && text[i] == message[j]) {
                j++;
            }
            if (j == m) {
                r = i;
                break;
            }
        }
        if (r != -1) {
            result += (n - r);
        }
    }
    
    cout << result << endl;
    
    return 0;
}
