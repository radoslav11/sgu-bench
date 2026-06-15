#include <iostream>
using namespace std;
int main(){
    int n; cin>>n;
    int result=n;
    int m=n;
    for(int p=2;p*p<=m;p++){
        if(m%p==0){
            while(m%p==0)m/=p;
            result-=result/p;
        }
    }
    if(m>1)result-=result/m;
    cout<<result<<endl;
}
