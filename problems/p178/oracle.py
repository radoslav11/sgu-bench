import sys

def main():
    data = sys.stdin.read().strip()
    if not data:
        return
    # Parse N (up to 1e16)
    n = int(data)

    # We'll track ans = number of links broken
    # and x = 2^(ans+1).  Start with ans=0 => x=2.
    ans = 0
    x = 2
    # We need the minimal ans such that (ans+1)*2^(ans+1)-1 >= n
    # Loop while we haven't reached n yet
    while x * (ans + 1) - 1 < n:
        ans += 1
        x <<= 1  # same as x = x * 2

    # Output the result
    print(ans)

if __name__ == "__main__":
    main()
