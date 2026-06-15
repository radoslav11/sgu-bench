#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<long long> x1(n), y1(n), x2(n), y2(n);
    vector<long long> dx(n), dy(n);
    
    for(int i = 0; i < n; i++){
        cin >> x1[i] >> y1[i] >> x2[i] >> y2[i];
        dx[i] = x2[i] - x1[i];
        dy[i] = y2[i] - y1[i];
    }
    
    long long count = 0;
    
    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            // Check if they share an endpoint
            bool share = false;
            if(x1[i]==x1[j] && y1[i]==y1[j]) share = true;
            else if(x1[i]==x2[j] && y1[i]==y2[j]) share = true;
            else if(x2[i]==x1[j] && y2[i]==y1[j]) share = true;
            else if(x2[i]==x2[j] && y2[i]==y2[j]) share = true;
            
            if(!share) continue;
            
            // Check perpendicularity
            long long dot = dx[i]*dx[j] + dy[i]*dy[j];
            if(dot == 0){
                count++;
            }
        }
    }
    
    cout << count << endl;
    
    return 0;
}
