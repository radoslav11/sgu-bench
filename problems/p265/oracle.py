import sys
import math


def identity_matrix():
    mat = [[0.0] * 4 for _ in range(4)]
    for i in range(4):
        mat[i][i] = 1.0
    return mat


def multiply(a, b):
    res = [[0.0] * 4 for _ in range(4)]
    for i in range(4):
        for j in range(4):
            s = 0.0
            for k in range(4):
                s += a[i][k] * b[k][j]
            res[i][j] = s
    return res


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx])
    idx += 1

    final = identity_matrix()
    for _ in range(n):
        typ = data[idx].decode()
        idx += 1
        cur = identity_matrix()

        if typ == 'T':
            tx = float(data[idx])
            ty = float(data[idx + 1])
            tz = float(data[idx + 2])
            idx += 3
            cur[0][3] = tx
            cur[1][3] = ty
            cur[2][3] = tz
        elif typ == 'S':
            sx = float(data[idx])
            sy = float(data[idx + 1])
            sz = float(data[idx + 2])
            idx += 3
            cur[0][0] = sx
            cur[1][1] = sy
            cur[2][2] = sz
        else:
            x = float(data[idx])
            y = float(data[idx + 1])
            z = float(data[idx + 2])
            angle = float(data[idx + 3])
            idx += 4
            length = math.sqrt(x * x + y * y + z * z)
            x /= length
            y /= length
            z /= length
            rad = angle * math.pi / 180.0
            co = math.cos(rad)
            si = math.sin(rad)
            one_minus_co = 1.0 - co
            cur[0][0] = co + x * x * one_minus_co
            cur[0][1] = x * y * one_minus_co - z * si
            cur[0][2] = x * z * one_minus_co + y * si
            cur[1][0] = y * x * one_minus_co + z * si
            cur[1][1] = co + y * y * one_minus_co
            cur[1][2] = y * z * one_minus_co - x * si
            cur[2][0] = z * x * one_minus_co - y * si
            cur[2][1] = z * y * one_minus_co + x * si
            cur[2][2] = co + z * z * one_minus_co

        final = multiply(cur, final)

    m = int(data[idx])
    idx += 1
    out = []
    for _ in range(m):
        px = float(data[idx])
        py = float(data[idx + 1])
        pz = float(data[idx + 2])
        idx += 3
        x = final[0][0] * px + final[0][1] * py + final[0][2] * pz + final[0][3]
        y = final[1][0] * px + final[1][1] * py + final[1][2] * pz + final[1][3]
        z = final[2][0] * px + final[2][1] * py + final[2][2] * pz + final[2][3]
        if abs(x) < 0.0005:
            x = 0.0
        if abs(y) < 0.0005:
            y = 0.0
        if abs(z) < 0.0005:
            z = 0.0
        out.append(f"{x:.2f} {y:.2f} {z:.2f}")

    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
