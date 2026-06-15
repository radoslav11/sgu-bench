blocks = []
lo, hi = L, R
while lo <= hi:
    d = 0, p = 1
    while p*10 <= hi-lo+1 and lo % (p*10) == 0:
        p *= 10; d += 1
    blocks.append((lo, d))  # block [lo, lo+p-1]
    lo += p
