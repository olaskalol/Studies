import numpy as np
from equations import functions_derivatives, Horner


def bisection(start_of_range, end_of_range, function, type_of_stop_condition , value):
    number_of_iterations=0
    middle_point = 0
    #checking if it is possible to perform bisection
    if function(start_of_range) * function(end_of_range) >= 0:
        print("Error: The function must have different signs at the endpoints of the interval.")
        return None

    # stop conditions
    if type_of_stop_condition== "epsilon":
        # trying to determinate zero of a function
        while True:
            number_of_iterations=number_of_iterations + 1
            prev_middle_point=middle_point
            middle_point = (start_of_range + end_of_range) / 2
            if number_of_iterations == 1 and function(middle_point) == 0:
                max_error = (end_of_range - start_of_range) / (2 ** (number_of_iterations + 1))
                return middle_point, number_of_iterations, max_error
            if (abs(middle_point-prev_middle_point) < value and number_of_iterations > 1) or abs(end_of_range - start_of_range) / 2 < value:
                max_error = (end_of_range - start_of_range) / (2 ** (number_of_iterations + 1))
                return middle_point, number_of_iterations, max_error
            #heart of bisection, division of the interval
            if function(start_of_range) * function(middle_point) < 0:
                end_of_range = middle_point
            else:
                start_of_range = middle_point
    #iteraction stop condition
    else:
        while number_of_iterations < value:
            number_of_iterations = number_of_iterations + 1
            middle_point = (start_of_range + end_of_range) / 2
            if function(middle_point) == 0:
                max_error = (end_of_range - start_of_range) / (2 ** (number_of_iterations + 1))
                return middle_point, number_of_iterations, max_error
            # heart of bisection, division of the interval
            if function(start_of_range) * function(middle_point) < 0:
                end_of_range = middle_point
            else:
                start_of_range = middle_point
    max_error = (end_of_range - start_of_range)/(2**(number_of_iterations+1))
    return middle_point, number_of_iterations, max_error
    #returing the most close value after max number of interactions

def secant_method(start_of_range, end_of_range, function, type_of_stop_condition, value, list_of_coefficients = None):

    if function(start_of_range) * function(end_of_range) >= 0:
        print("Error: The function must have different signs at the endpoints of the interval.")
        return None

    x1 = start_of_range
    y1 = function(x1)
    x2 = end_of_range
    y2 = function(x2)
    x0 = x1
    iterations = 0
    previous_step = abs(end_of_range - start_of_range)
    error = 0

    while True:
        if type_of_stop_condition == "epsilon" and abs(x1 - x2) < value:
            break
        if type_of_stop_condition == "iterations" and iterations >= value:
            break
        if y1 - y2 == 0: #zabezpieczenie przed poziomą sieczną
            print("Error: Horizontal secant")
            break

        x0 = x1 - y1 * (x1 - x2) / (y1 - y2)
        current_step = abs(x0 - x1)

        if current_step > 10 * previous_step: #gwałtowny przyrost między przybliżeniami - wskazuje rozbieżność
            print("Error: Discrepancy")
            return None

        previous_step = current_step
        error = secant_error(x0, function, start_of_range, end_of_range)

        f0 = function(x0)
        x2 = x1
        y2 = y1
        x1 = x0
        y1 = f0
        iterations += 1

    if x0 < start_of_range or x0 > end_of_range:
        print("Error: It was not possible to find the point of zero at given range")
        return None

    return x0, iterations, error

def secant_error(xn, function, start_of_range, end_of_range, list_of_coefficients = None):
    if xn is None:
        return None
    dots_x = np.linspace(start_of_range, end_of_range, 10000)
    if function in functions_derivatives:
        derivatives = [abs(functions_derivatives[function](x)) for x in dots_x]
    else:
        if list_of_coefficients is not None:
            n = len(list_of_coefficients) - 1
            der_coeffs = [list_of_coefficients[i] * (n - i) for i in range(n)]
            if not der_coeffs:
                derivatives = [0] * len(dots_x)
            else:
                derivatives = [abs(Horner(x, der_coeffs)) for x in dots_x]
        else:
            return None
    m = np.min(derivatives)
    if m < 1e-12:
        return "Inf (m ≈ 0)"
    error = abs(function(xn)) / m
    return error