import sys


def solve_case(x, y, z):
    a, b, c = sorted((x, y, z))

    edges = (x - 1) * y * z + x * (y - 1) * z + x * y * (z - 1)

    if a == 1 and b == 1:
        extra = 0
    elif a == 1:
        both_odd = b % 2 == 1 and c % 2 == 1
        extra = max(0, b + c - 5 + int(both_odd))
    else:
        base = (a - 2) * (b - 2) + (a - 2) * (c - 2) + (b - 2) * (c - 2)
        if a == 2:
            correction = 3
        elif a == 3:
            correction = [6, 7, 9][(b % 2) + (c % 2)]
        else:
            correction = 9
        extra = base + correction

    return edges + extra


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    out = []
    case_no = 1
    for i in range(0, len(data), 3):
        x, y, z = data[i], data[i + 1], data[i + 2]
        out.append(f"Case #{case_no}: {solve_case(x, y, z)}")
        case_no += 1
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
