#include<bits/stdc++.h>
using std.h>

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    cin.ignore();
    
    vector<int> scores(M+1);
    for(int i = 1; i <= M; i++){
        cin >> scores[i];
    }
    
    vector<pair<string,string>> accepted;
    
    for(int i = 0; i < N; i++){
        string facultyName;
        getline(cin, facultyName);
        
        int K;
        cin >> K;
        vector<int> subjects(K);
        for(int j = 0; j < K; j++){
            cin >> subjects[j];
        }
        
        vector<tuple<int,int,int>> ranges(K);
        for(int j = 0; j < K; j++){
            int x, y, z;
            cin >> x >> y >> z;
            ranges[j] = make_tuple(x, y, z);
        }
        cin.ignore();
        
        int S;
        cin >> S;
        cin.ignore();
        
        for(int j = 0; j < S; j++){
            string specialtyName;
            getline(cin, specialtyName);
            
            int minSum;
            cin >> minSum;
            cin.ignore();
            
            // Calculate grade sum
            int gradeSum = 0;
            for(int k = 0; k < K; k++){
                int score = scores[subjects[k]];
                int x = get<0>(ranges[k]);
                int y = get<1>(ranges[k]);
                int z = get<2>(ranges[k]);
                
                int grade;
                if(score < x) grade = 2;
                else if(score < y) grade = 3;
                else if(score < z) grade = 4;
                else grade = 5;
                
                gradeSum += grade;
            }
            
            if(gradeSum >= minSum){
                accepted.push_back({facultyName, specialtyName});
            }
        }
    }
    
    if(accepted.empty()){
        cout << "Army\n";
    } else {
        for(auto& p : accepted){
            cout << p.first << " " << p.second << "\n";
        }
    }
    
    return 0;
}
