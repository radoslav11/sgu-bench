#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, M;
    {
        string line;
        getline(cin, line);
        stringstream ss(line);
        ss >> N >> M;
    }
    vector<int> B(M);
    {
        string line;
        getline(cin, line);
        stringstream ss(line);
        for(int i=0;i<M;i++) ss>>B[i];
    }
    
    vector<string> results;
    
    for(int f=0; f<N; f++){
        string facName;
        getline(cin, facName);
        int K;
        vector<int> subj;
        {
            string line;
            getline(cin, line);
            stringstream ss(line);
            ss >> K;
            subj.resize(K);
            for(int i=0;i<K;i++) ss>>subj[i];
        }
        int totalGrade = 0;
        {
            string line;
            getline(cin, line);
            stringstream ss(line);
            for(int i=0;i<K;i++){
                int X,Y,Z;
                ss>>X>>Y>>Z;
                int T = B[subj[i]-1];
                int g;
                if(T < X) g=2;
                else if(T < Y) g=3;
                else if(T < Z) g=4;
                else g=5;
                totalGrade += g;
            }
        }
        int S;
        {
            string line;
            getline(cin, line);
            stringstream ss(line);
            ss >> S;
        }
        for(int s=0;s<S;s++){
            string specName;
            getline(cin, specName);
            int req;
            {
                string line;
                getline(cin, line);
                stringstream ss(line);
                ss >> req;
            }
            if(totalGrade >= req){
                results.push_back(facName + " " + specName);
            }
        }
    }
    
    if(results.empty()){
        cout << "Army" << "\n";
    } else {
        for(auto &r : results) cout << r << "\n";
    }
    return 0;
}
