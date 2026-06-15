#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    map<string, int> menIdx, womenIdx;
    vector<string> menNames(n), womenNames(n);
    vector<vector<int>> menPrefs(n), womenPrefs(n);
    
    // Read men preferences
    for(int i = 0; i < n; i++){
        string man;
        cin >> man;
        menNames[i] = man;
        menIdx[man] = i;
        menPrefs[i].resize(n);
        for(int j = 0; j < n; j++){
            string woman;
            cin >> woman;
            if(i == 0) womenIdx[woman] = j;
            menPrefs[i][j] = womenIdx.count(woman) ? womenIdx[woman] : -1;
        }
    }
    
    // Read women preferences
    for(int i = 0; i < n; i++){
        string woman;
        cin >> woman;
        womenNames[i] = woman;
        womenPrefs[i].resize(n);
        for(int j = 0; j < n; j++){
            string man;
            cin >> man;
            womenPrefs[i][j] = menIdx[man];
        }
    }
    
    // Build ranking matrix for women (for O(1) preference comparison)
    vector<vector<int>> womenRank(n, vector<int>(n));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            womenRank[i][womenPrefs[i][j]] = j;
        }
    }
    
    // Gale-Shapley algorithm
    vector<int> womanPartner(n, -1);  // which man is woman i with (-1 if free)
    vector<int> manPartner(n, -1);    // which woman is man i with (-1 if free)
    vector<int> nextProposal(n, 0);   // next woman index to propose to for man i
    
    queue<int> freeMen;
    for(int i = 0; i < n; i++){
        freeMen.push(i);
    }
    
    while(!freeMen.empty()){
        int man = freeMen.front();
        freeMen.pop();
        
        if(nextProposal[man] < n){
            int woman = menPrefs[man][nextProposal[man]];
            nextProposal[man]++;
            
            if(womanPartner[woman] == -1){
                // Woman is free
                womanPartner[woman] = man;
                manPartner[man] = woman;
            } else {
                // Woman is engaged
                int currentPartner = womanPartner[woman];
                if(womenRank[woman][man] < womenRank[woman][currentPartner]){
                    // Woman prefers new man
                    womanPartner[woman] = man;
                    manPartner[man] = woman;
                    manPartner[currentPartner] = -1;
                    freeMen.push(currentPartner);
                } else {
                    // Woman prefers current partner
                    freeMen.push(man);
                }
            }
        }
    }
    
    cout << "YES\n";
    for(int i = 0; i < n; i++){
        cout << menNames[i] << " " << womenNames[manPartner[i]] << "\n";
    }
    
    return 0;
}
