GAUSS_HERMITE_DATA = {
    2: {
        "nodes": [-0.707107, 0.707107],
        "weights": [0.886227, 0.886227]
    },
    3: {
        "nodes": [-1.224745, 0, 1.224745],
        "weights": [0.295409, 1.181636, 0.295409]
    },
    4: {
        "nodes": [-1.650680, -0.534648, 0.534648, 1.650680],
        "weights": [0.081313, 0.804914, 0.804914, 0.081313]
    },
    5: {
        "nodes": [-2.020183, -0.958572, 0, 0.958572, 2.020183],
        "weights": [0.019953, 0.393619, 0.945309, 0.393619, 0.019953]
    }
}

def gauss_hermite_quadrature(f):
    results = {}
    for n in GAUSS_HERMITE_DATA.keys():
        data = GAUSS_HERMITE_DATA[n]
        integral = 0
        for x_i, A_i in zip(data["nodes"], data["weights"]):
            integral += A_i * f(x_i)
        results[n] = integral
    return results