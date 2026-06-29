import numpy as np
import matplotlib.pyplot as plt
from Newton_method import Newton_method


def plot_summary(nodes, start_of_range, end_of_range, label, function):
    _, X, Y, coefficients_a = Newton_method(0, nodes)

    name_label = "f(x) = "
    for i in range(len(coefficients_a)):
        coef = round(coefficients_a[i], 3)
        power = len(coefficients_a) - 1 - i
        name_label += f"{coef}*x^{power} + "
    name_label = name_label[:-3]

    x_vec = np.linspace(start_of_range, end_of_range, 500)
    y_vec_original = np.vectorize(function)(x_vec)
    y_vec_polynomial = [Newton_method(x_val, nodes)[0] for x_val in x_vec]

    plt.figure(figsize=(12, 8))

    plt.plot(x_vec, y_vec_original, label=f"Oryginalna funkcja: {label}", color='green', linewidth=3)

    plt.plot(x_vec, y_vec_polynomial, label=f"Wielomian:\n{name_label}", color='blue', linewidth=2, linestyle='--')

    fixed_X =[]
    fixed_Y = []
    for i in range(len(X)):
        if end_of_range > X[i] > start_of_range:
            fixed_X.append(X[i])
            fixed_Y.append(Y[i])

    plt.scatter(fixed_X, fixed_Y, color='red', s=40, zorder=5, label="Węzły interpolacji")

    plt.axhline(0, color='black', linewidth=1)
    plt.axvline(0, color='black', linewidth=1, alpha=0.3)
    plt.grid(True, linestyle=':', alpha=0.6)

    plt.title(f"Porównanie: {label} oraz wielomianu interpolacyjnego")
    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.legend(loc="best")
    plt.show()