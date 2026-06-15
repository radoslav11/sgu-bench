import sys

def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    n = int(data[0])  
    # The maximum number of regions formed by n lines in general position
    # is 1 plus the sum of integers from 1 to n:
    #   regions = 1 + (1 + 2 + ... + n) = 1 + n*(n+1)//2
    result = 1 + n * (n + 1) // 2
    print(result)

if __name__ == "__main__":
    main()
