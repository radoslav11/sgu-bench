import sys


def main():
    data = sys.stdin.read().strip().split()
    n = int(data[0])

    # spf[i] is the smallest prime factor of i; Omega[i] counts prime
    # factors of i with multiplicity, and dp[i] = Omega[i] + 1 is the
    # length of the longest divisor chain ending at i.
    spf = [0] * (n + 1)
    omega = [0] * (n + 1)
    for i in range(2, n + 1):
        if spf[i] == 0:
            for j in range(i, n + 1, i):
                if spf[j] == 0:
                    spf[j] = i

    for i in range(2, n + 1):
        omega[i] = omega[i // spf[i]] + 1

    dp = [omega[i] + 1 for i in range(n + 1)]
    m = max(dp[1:])
    out = [str(m), " ".join(str(dp[i]) for i in range(1, n + 1))]
    print("\n".join(out))


if __name__ == "__main__":
    main()
