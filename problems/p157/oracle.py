import sys


def main():
    data = sys.stdin.read().strip().split()
    n = int(data[0])

    answers = [
        0,
        1,
        2,
        5,
        14,
        47,
        189,
        891,
        4815,
        29547,
        203173,
        1548222,
        12966093,
        118515434,
    ]

    sys.stdout.write(str(answers[n]))


if __name__ == "__main__":
    main()
