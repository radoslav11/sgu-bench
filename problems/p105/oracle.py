import sys


def main():
    n = int(sys.stdin.read().strip())

    full_groups = n // 3
    answer = full_groups * 2
    if n % 3 == 2:
        answer += 1

    print(answer)


if __name__ == "__main__":
    main()
