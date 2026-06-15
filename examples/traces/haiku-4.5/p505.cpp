#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<string> genomes(n);
    for(int i = 0; i < n; i++){
        cin >> genomes[i];
    }
    
    int m;
    cin >> m;
    
    for(int i = 0; i < m; i++){
        string prefix, suffix;
        cin >> prefix >> suffix;
        
        int count = 0;
        for(int j = 0; j < n; j++){
            const string& genome = genomes[j];
            
            // Check if genome has prefix and suffix
            bool hasPrefix = false;
            bool hasSuffix = false;
            
            if(genome.length() >= prefix.length()){
                hasPrefix = (genome.substr(0, prefix.length()) == prefix);
            }
            
            if(genome.length() >= suffix.length()){
                hasSuffix = (genome.substr(genome.length() - suffix.length()) == suffix);
            }
            
            if(hasPrefix && hasSuffix){
                count++;
            }
        }
        
        cout << count << "\n";
    }
    
    return 0;
}
