def solve():
    # Read the target sum n
    n = int(input().strip())
    
    # Base splits off as many full '14 per die' as possible
    k = n // 14
    rem = n % 14
    
    # Case k=1: only possible if the single die shows all faces => sum=21
    if k == 1 and n != 21:
        print(-1)
        return
    
    # If k=0 => n < 14 (and only n=21 works but that's k=1)
    # If remainder rem <= 1 or rem >= 13 => no way to choose top+bottom faces in [1..6]
    if k == 0 or rem <= 1 or rem >= 13:
        print(-1)
    else:
        # We can pick two face-values x,y in [1..6] summing to rem (2..12)
        print(k)

if __name__ == "__main__":
    solve()
