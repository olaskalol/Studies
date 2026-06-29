def correctMatrixMaking(x, function):
    matrix = []
    for i in range(0, len(x)):
        matrix.append([x[i], function(x[i])])
    return matrix


def Newton_method(x, matrix):
    X = []
    Y = []
    for row in matrix:
        X.append(row[0])
        Y.append(row[1])
    n = len(matrix)
    coefficients_a = Y.copy()
    for j in range(1, n):
        for i in range(n - 1, j - 1, -1):
            # Wzór na iloraz:
            coefficients_a[i] = (coefficients_a[i] - coefficients_a[i - 1]) / (X[i] - X[i - j])

    polynomial = coefficients_a[0]
    brackets = 1.0

    for i in range(1, len(coefficients_a)):
        node_x = X[i - 1]
        brackets = brackets * (x - node_x)
        polynomial = polynomial + (coefficients_a[i] * brackets)

    return polynomial,X,Y,coefficients_a