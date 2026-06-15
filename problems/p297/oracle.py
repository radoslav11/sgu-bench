import sys

def main():
    data = sys.stdin.read().split()
    # First two numbers: N friends, M pubs
    N, M = map(int, data[:2])
    # Next M numbers: the bills S1..SM
    bills = map(int, data[2:])
    
    total_mod = 0
    # Sum each bill modulo N to avoid large sums
    for s in bills:
        total_mod = (total_mod + s) % N
    
    # The answer is (sum of all bills) mod N
    print(total_mod)

if __name__ == "__main__":
    main()
