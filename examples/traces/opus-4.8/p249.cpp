#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, M;
    scanf("%d %d", &N, &M);
    int rows = 1<<N;
    int cols = 1<<M;
    
    vector<long long> grayRow(rows), grayCol(cols);
    for(int i=0;i<rows;i++) grayRow[i] = i ^ (i>>1);
    for(int j=0;j<cols;j++) grayCol[j] = j ^ (j>>1);
    
    long long base = 1LL<<N;
    
    string out;
    out.reserve((size_t)rows*cols*4);
    char buf[24];
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            long long val = grayCol[j]*base + grayRow[i];
            int len = sprintf(buf, "%lld", val);
            out.append(buf, len);
            if(j+1<cols) out.push_back(' ');
        }
        out.push_back('\n');
    }
    fputs(out.c_str(), stdout);
    return 0;
}
