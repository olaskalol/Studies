import numpy as np

def gauss_hermite_quadrature(f, n_nodes):
    nodes, weights = np.polynomial.hermite.hermgauss(n_nodes)
    integral = 0.0
    for x_i, A_i in zip(nodes, weights):
        integral += A_i * f(x_i)

    return integral