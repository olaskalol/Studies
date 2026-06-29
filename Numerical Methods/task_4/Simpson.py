def simpson_integration(func, a, b, N):
    if N % 2 != 0:
        N += 1
    h = (b - a) / N
    suma = func(a) + func(b)

    for i in range(1, N):
        xi = a + i * h
        if i % 2 == 0:
            suma += 2 * func(xi)
        else:
            suma += 4 * func(xi)

    return (h / 3) * suma


def simpson_finite(function, a, b, eps):
    N = 2
    prev = simpson_integration(function, a, b, N)
    MAX_ITERS = 25

    for _ in range(MAX_ITERS):
        N *= 2
        new = simpson_integration(function, a, b, N)
        if abs(new - prev) < eps:
            return new
        prev = new

    return prev

def simpson_derivative(func, eps, delta=1.0):
    sum = 0
    a = 0
    MAX_STEPS = 1000
    for _ in range(MAX_STEPS):
        b = a + delta
        curr_block = simpson_finite(func, a, b, eps)
        if abs(curr_block) < eps:
            break
        sum += curr_block
        a = b

    b = 0
    for _ in range(MAX_STEPS):
        a = b - delta
        curr_block = simpson_finite(func, a, b, eps)
        if abs(curr_block) < eps:
            break
        sum += curr_block
        b = a

    return sum