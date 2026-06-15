Big mulB(const Big&a,const Big&b){
    int n=a.size()+b.size();
    vector<__int128>r(n,0);
    for(int i=0;i<(int)a.size();i++)
        for(int j=0;j<(int)b.size();j++)
            r[i+j]+=(__int128)a[i]*b[j];
    Big res(n);
    __int128 c=0;
    for(int i=0;i<n;i++){r[i]+=c;res[i]=r[i]%BASE;c=r[i]/BASE;}
    while(res.size()>1&&res.back()==0)res.pop_back();
    return res;
}
