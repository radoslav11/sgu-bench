#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int n;
    cin >> n;
    
    map<vector<pair<string, string>>, int> freq;
    
    for (int i = 0; i < n; i++) {
        int ki;
        cin >> ki;
        vector<pair<string, string>> props;
        for (int j = 0; j < ki; j++) {
            string token;
            cin >> token;
            size_t pos = token.find('=');
            string key = token.substr(0, pos);
            string value = token.substr(pos + 1);
            props.push_back(make_pair(key, value));
        }
        sort(props.begin(), props.end());
        for (int mask = 1; mask < (1 << ki); mask++) {
            vector<pair<string, string>> subset;
            for (int j = 0; j < ki; j++) {
                if (mask & (1 << j)) {
                    subset.push_back(props[j]);
                }
            }
            freq[subset]++;
        }
    }
    
    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        int li;
        cin >> li;
        vector<pair<string, string>> query_props;
        for (int j = 0; j < li; j++) {
            string token;
            cin >> token;
            size_t pos = token.find('=');
            string key = token.substr(0, pos);
            string value = token.substr(pos + 1);
            query_props.push_back(make_pair(key, value));
        }
        sort(query_props.begin(), query_props.end());
        auto it = freq.find(query_props);
        if (it != freq.end()) {
            cout << it->second << '\n';
        } else {
            cout << 0 << '\n';
        }
    }
    
    return 0;
}
