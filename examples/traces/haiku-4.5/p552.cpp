#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int n;
  cin >> n;
  
  vector<map<string, string>> objects(n);
  
  // Read objects
  for(int i = 0; i < n; i++){
    int k;
    cin >> k;
    for(int j = 0; j < k; j++){
      string prop;
      cin >> prop;
      // Parse key=value
      size_t eqPos = prop.find('=');
      string key = prop.substr(0, eqPos);
      string value = prop.substr(eqPos + 1);
      objects[i][key] = value;
    }
  }
  
  int m;
  cin >> m;
  
  // Process queries
  for(int i = 0; i < m; i++){
    int l;
    cin >> l;
    
    map<string, string> queryProps;
    for(int j = 0; j < l; j++){
      string prop;
      cin >> prop;
      // Parse key=value
      size_t eqPos = prop.find('=');
      string key = prop.substr(0, eqPos);
      string value = prop.substr(eqPos + 1);
      queryProps[key] = value;
    }
    
    // Count matching objects
    int count = 0;
    for(int j = 0; j < n; j++){
      bool matches = true;
      // Check if object j has all properties from query
      for(auto& [qKey, qValue] : queryProps){
        if(objects[j].find(qKey) == objects[j].end() || 
           objects[j][qKey] != qValue){
          matches = false;
          break;
        }
      }
      if(matches){
        count++;
      }
    }
    
    cout << count << "\n";
  }
  
  return 0;
}
