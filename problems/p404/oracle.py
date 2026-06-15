def main():
    import sys
    data = sys.stdin.read().split()
    N = int(data[0])
    M = int(data[1])
    phrases = data[2:]

    idx = (N - 1) % M
    print(phrases[idx])


if __name__ == "__main__":
    main()
