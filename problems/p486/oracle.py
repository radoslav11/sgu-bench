secret = input().strip()
guess = input().strip()

bulls = sum(1 for i in range(4) if secret[i] == guess[i])

secret_digits = set(secret)

matches = sum(1 for ch in guess if ch in secret_digits)

cows = matches - bulls

print(bulls, cows)
