import sys


def main():
    n = int(sys.stdin.read().split()[0])

    limit = max(n, 10)
    # Sieve, then collect primes whose 1-based index is itself prime.
    sieve_limit = 110000
    sieve = bytearray([1]) * (sieve_limit + 1)
    sieve[0] = sieve[1] = 0
    for i in range(2, int(sieve_limit**0.5) + 1):
        if sieve[i]:
            sieve[i * i::i] = bytearray(len(sieve[i * i::i]))
    primes = [i for i in range(sieve_limit + 1) if sieve[i]]
    sps = [p for idx, p in enumerate(primes, start=1)
           if p <= limit and idx < len(sieve) and sieve[idx]]

    INF = float("inf")
    dp = [INF] * (n + 1)
    dp[0] = 0
    for v in sps:
        for j in range(v, n + 1):
            if dp[j - v] + 1 < dp[j]:
                dp[j] = dp[j - v] + 1

    if dp[n] == INF:
        print(0)
        return

    items = []
    cur = n
    while cur:
        for v in sps:
            if cur >= v and dp[cur - v] == dp[cur] - 1:
                items.append(v)
                cur -= v
                break

    items.sort(reverse=True)
    print(len(items))
    print(" ".join(map(str, items)))


if __name__ == "__main__":
    main()
