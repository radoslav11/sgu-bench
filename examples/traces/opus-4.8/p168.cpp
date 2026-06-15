diag[i][j] = A[i][j] if i==1 or j==M else min(A[i][j], diag[i-1][j+1])
B[i][j] = min(diag[i][j], (i<N? B[i+1][j] : INF))
