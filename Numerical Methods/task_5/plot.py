import numpy as np
import matplotlib.pyplot as plt
from equations import Horner


def plot_approximation_summary(start_of_range, end_of_range, label, function, coefficients, error_val, degree):
    name_label = f"P_{degree}(x) = "
    for i in range(len(coefficients) - 1, -1, -1):
        coef = round(coefficients[i], 3)
        if coef != 0 or len(coefficients) == 1:
            name_label += f"{coef}*x^{i} + "

    if name_label.endswith(" + "):
        name_label = name_label[:-3]

    x_vec = np.linspace(start_of_range, end_of_range, 500)
    y_vec_original = np.vectorize(function)(x_vec)
    y_vec_polynomial = [Horner(x_val, coefficients[::-1]) for x_val in x_vec]


    plt.figure(figsize=(12, 8))
    plt.plot(x_vec, y_vec_original, label=f"Oryginalna: {label}", color='green', linewidth=3)
    plt.plot(x_vec, y_vec_polynomial, label=f"Aproksymacja:\n{name_label}", color='red', linewidth=2, linestyle='--')

    plt.axhline(0, color='black', linewidth=1)
    if start_of_range <= 0 <= end_of_range:
        plt.axvline(0, color='black', linewidth=1, alpha=0.3)

    plt.grid(True, linestyle=':', alpha=0.6)
    plt.xlim(start_of_range-1, end_of_range+1)

    min_y = np.min(y_vec_original)
    max_y = np.max(y_vec_original)
    margin = abs(max_y - min_y) if abs(max_y - min_y) != 0 else 1
    plt.ylim(min_y - 2 * margin, max_y + 2 * margin)

    error_text = f"Stopień: {degree}\nBłąd aproksymacji: {error_val:.5e}"
    plt.text(0.02, 0.95, error_text, transform=plt.gca().transAxes,
             fontsize=12, verticalalignment='top',
             bbox=dict(boxstyle='round,pad=0.5', facecolor='white', edgecolor='red', alpha=0.9))

    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.legend(loc="lower right")
    plt.show()