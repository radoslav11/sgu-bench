#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<string> strings(n);
    for (int i = 0; i < n; i++) {
        cin >> strings[i];
    }
    
    // Custom comparator: a comes before b if a+b < b+a
    sort(strings.begin(), strings.end(), [](const string& a, const string& b) {
        return a + b < b + a;
    });
    
    // Concatenate all strings
    string result = "";
    for (const string& s : strings) {
        result += s;
    }
    
    cout << result << endl;
    
    return 0;
}
