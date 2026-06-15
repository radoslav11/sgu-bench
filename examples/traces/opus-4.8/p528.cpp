#include <bits/stdc++.h>
using namespace std;

int main(){
    // fast input
    long long m;
    // read m
    scanf("%lld", &m);
    // read the sequence: it's on the second line. Could be up to 1e6 chars.
    // read rest. Need to skip whitespace/newline after m, then read line.
    static char buf[1000006];
    // read line
    // use scanf? characters include ascii 33-127, no spaces (33 is '!', space is 32 excluded). So %s works (no spaces).
    scanf("%s", buf);
    int len = strlen(buf);
    
    enum State { S_OBJ_REQUIRED, S_ELEM_OR_END, S_DONE, S_INT_START, S_INT_ZERO, S_INT_DIGITS, S_STRLEN_ZERO, S_STRLEN_DIGITS, S_STRBODY };
    
    int ms = S_OBJ_REQUIRED;
    long long lenVal = 0;
    long long remaining = 0;
    
    // stack
    // frame type: 0 = LIST, 1 = DICT. pendingKey stored.
    static char ftype[1000006];
    static char fpend[1000006];
    int sp = 0; // stack size = nf
    
    auto onComplete = [&](){
        if(sp==0){ ms = S_DONE; }
        else {
            if(ftype[sp-1]==0){ ms = S_ELEM_OR_END; }
            else {
                if(fpend[sp-1]==0){ fpend[sp-1]=1; ms = S_OBJ_REQUIRED; }
                else { fpend[sp-1]=0; ms = S_ELEM_OR_END; }
            }
        }
    };
    
    int answer = -1; // position of error, -1 means none yet
    
    for(int i=0;i<len;i++){
        char ch = buf[i];
        bool invalid = false;
        switch(ms){
            case S_DONE:
                invalid = true; break;
            case S_OBJ_REQUIRED:
            case S_ELEM_OR_END:
                if(ms==S_ELEM_OR_END && ch=='e'){
                    // close innermost frame
                    sp--; // pop
                    onComplete();
                } else if(ch=='i'){ ms=S_INT_START; }
                else if(ch=='l'){ ftype[sp]=0; fpend[sp]=0; sp++; ms=S_ELEM_OR_END; }
                else if(ch=='d'){ ftype[sp]=1; fpend[sp]=0; sp++; ms=S_ELEM_OR_END; }
                else if(ch>='0'&&ch<='9'){
                    if(ch=='0'){ ms=S_STRLEN_ZERO; }
                    else { ms=S_STRLEN_DIGITS; lenVal = ch-'0'; }
                } else invalid=true;
                break;
            case S_INT_START:
                if(ch=='0'){ ms=S_INT_ZERO; }
                else if(ch>='1'&&ch<='9'){ ms=S_INT_DIGITS; }
                else invalid=true;
                break;
            case S_INT_ZERO:
                if(ch=='e'){ onComplete(); }
                else invalid=true;
                break;
            case S_INT_DIGITS:
                if(ch>='0'&&ch<='9'){ /* stay */ }
                else if(ch=='e'){ onComplete(); }
                else invalid=true;
                break;
            case S_STRLEN_ZERO:
                if(ch==':'){ onComplete(); }
                else invalid=true;
                break;
            case S_STRLEN_DIGITS:
                if(ch>='0'&&ch<='9'){ lenVal = lenVal*10 + (ch-'0'); }
                else if(ch==':'){ remaining = lenVal; ms=S_STRBODY; /* lenVal>=1 so remaining>=1 */ }
                else invalid=true;
                break;
            case S_STRBODY:
                // any char accepted
                remaining--;
                if(remaining==0){ onComplete(); }
                break;
        }
        
        if(invalid){ answer = i; break; }
        
        // compute minRem for new state
        long long nf = sp;
        long long minRem;
        switch(ms){
            case S_DONE: minRem = 0; break;
            case S_OBJ_REQUIRED: minRem = 2 + nf; break;
            case S_ELEM_OR_END: minRem = nf; break;
            case S_INT_START: minRem = 2 + nf; break;
            case S_INT_ZERO: minRem = 1 + nf; break;
            case S_INT_DIGITS: minRem = 1 + nf; break;
            case S_STRLEN_ZERO: minRem = 1 + nf; break;
            case S_STRLEN_DIGITS: minRem = 1 + lenVal + nf; break;
            case S_STRBODY: minRem = remaining + nf; break;
            default: minRem = 0;
        }
        long long total = (long long)(i+1) + minRem;
        if(total > m){ answer = i; break; }
    }
    
    if(answer==-1){
        // consumed all chars
        if(ms==S_DONE) printf("ok\n");
        else printf("Error at position %d!\n", len);
    } else {
        printf("Error at position %d!\n", answer);
    }
    return 0;
}
