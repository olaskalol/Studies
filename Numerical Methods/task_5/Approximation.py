import math
from GaussaHermita import gauss_hermite_quadrature
from equations import Horner


def get_hermite_natural_coeffs(max_k):
    H = [[1.0]]
    if max_k >= 1:
        H.append([0.0, 2.0])

    for k in range(2, max_k + 1):
        coeffs = [0.0] * (k + 1)
        for i in range(k):
            coeffs[i + 1] += 2.0 * H[k - 1][i]
        for i in range(k - 1):
            coeffs[i] -= 2.0 * (k - 1) * H[k - 2][i]
        H.append(coeffs)
    return H


def get_coefficients(func, deg, num_nodes):
    H_coeffs = get_hermite_natural_coeffs(deg)
    c_array = []

    for k in range(deg + 1):
        func_to_integrate = lambda x, k=k: func(x) * Horner(x, H_coeffs[k][::-1])

        integral_numerator = gauss_hermite_quadrature(func_to_integrate, num_nodes)
        integral_denominator = math.sqrt(math.pi) * (2 ** k) * math.factorial(k)
        c_k = integral_numerator / integral_denominator
        c_array.append(c_k)

    A = [0.0] * (deg + 1)
    for k in range(deg + 1):
        for i in range(k + 1):
            A[i] += c_array[k] * H_coeffs[k][i]

    return A


def calculate_approximation_error(function, coefficients, n_nodes=5):
    def error_func(x):
        difference = function(x) - Horner(x, coefficients[::-1])
        return difference ** 2

    integral_value = gauss_hermite_quadrature(error_func, n_nodes)

    return math.sqrt(max(integral_value, 0))


def auto_approximate(func, expected_error, max_deg=15, n_nodes=5):
    H_coeffs = get_hermite_natural_coeffs(max_deg)
    c_array = []
    deg = 0
    current_error = float('inf')
    A_final = []

    while current_error > expected_error:
        deg += 1
        if deg > max_deg:
            print(f"\nm={max_deg}. E: {current_error:.5e}")
            break

        start_k = len(c_array)
        for k in range(start_k, deg + 1):
            func_to_integrate = lambda x, k=k: func(x) * Horner(x, H_coeffs[k][::-1])

            integral_numerator = gauss_hermite_quadrature(func_to_integrate, n_nodes)
            integral_denominator = math.sqrt(math.pi) * (2 ** k) * math.factorial(k)
            c_array.append(integral_numerator / integral_denominator)

        A_final = [0.0] * (deg + 1)
        for k in range(deg + 1):
            for i in range(k + 1):
                A_final[i] += c_array[k] * H_coeffs[k][i]

        current_error = calculate_approximation_error(func, A_final, n_nodes)

    return deg, A_final, current_error