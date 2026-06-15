#include <bits/stdc++.h>
using namespace std;

static const int MAXN = 1000001;
long long cube[MAXN+2];
int startBIT[MAXN+2];
int endBIT[MAXN+2];
long long cubeBIT[MAXN+2];
int N = MAXN; // BIT size for start/end (cells 1..1000000). use MAXN-1? cells up to 1e6.
