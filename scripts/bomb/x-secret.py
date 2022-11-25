n48 = [0x3e9, None, None]
n46 = [0x2e, None, None]
n43 = [0x14, None, None]
n42 = [0x7, None, None]
n44 = [0x23, None, None]
n47 = [0x63, None, None]
n41 = [0x1, None, None]
n45 = [0x28, None, None]

n34 = [0x6b, n47, n48]
n31 = [0x06, n41, n42]
n33 = [0x2d, n45, n46]
n32 = [0x16, n43, n44]

n21 = [0x08, n31, n32]
n22 = [0x32, n33, n34]

n1 = [0x24, n21, n22]

it = set()


def fun7(p1, p2):
    i = 0
    if p1 == None:
        i = -1
    elif p2 < p1[0]:
        i = fun7(p1[1], p2)
        i *= 2
    elif p2 == p1[0]:
        i = 0
    else:
        i = fun7(p1[2], p2)
        i = i*2+1
    return i


if __name__ == "__main__":
    for i in range(0, 1002):
        if 7 > fun7(n1, i) > 0:
            if fun7(n1, i) not in it:
                print(fun7(n1, i))
                it.add(fun7(n1, i))
