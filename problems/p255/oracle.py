import math
import sys


def is_triangular(t: int) -> bool:
    if t < 0:
        return False
    D = 1 + 8 * t
    s = int(math.isqrt(D))
    if s * s != D:
        return False
    if (s - 1) % 2 != 0:
        return False
    k = (s - 1) // 2
    return k >= 1 and k * (k + 1) // 2 == t


def solve_case(M: int) -> str:
    if M < 2:
        return "NO"
    return "YES" if is_triangular(M - 1) else "NO"


def main() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    T = int(data[0])
    out = []
    for i in range(1, 1 + T):
        out.append(solve_case(int(data[i])))
    sys.stdout.write("\n".join(out) + ("\n" if out else ""))


if __name__ == "__main__":
    main()
