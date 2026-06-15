#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M; cin>>N>>M;
    vector<int> B(M+1);
    for(int i=1;i<=M;i++) cin>>B[i];
    cin.ignore();
    vector<pair<string,string>> accepted;
    for(int n=0;n<N;n++){
        string facultyName; getline(cin,facultyName);
        int K; cin>>K;
        vector<int> subjects(K);
        for(int i=0;i<K;i++) cin>>subjects[i];
        vector<int> X(K),Y(K),Z(K);
        for(int i=0;i<K;i++) cin>>X[i]>>Y[i]>>Z[i];
        int totalGrade=0;
        for(int i=0;i<K;i++){
            int T=B[subjects[i]];
            int g;
            if(T<X[i]) g=2;
            else if(T<Y[i]) g=3;
            else if(T<Z[i]) g=4;
            else g=5;
            totalGrade+=g;
        }
        int S; cin>>S; cin.ignore();
        for(int s=0;s<S;s++){
            string spec; getline(cin,spec);
            int req; cin>>req; cin.ignore();
            if(totalGrade>=req) accepted.push_back({facultyName,spec});
        }
    }
    if(accepted.empty()) cout<<"Army\n";
    else for(auto&p:accepted) cout<<p.first<<" "<<p.second<<"\n";
}
