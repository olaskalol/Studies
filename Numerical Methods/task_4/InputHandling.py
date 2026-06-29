from equations import *
from Simpson import *
from GaussaHermita import *
from table import table_summary

def check_convergence_symbolic(g_function):
    try:
        test_pos = g_function(50)
        test_neg = g_function(-50)

        if abs(test_pos) < 1e-10 and abs(test_neg) < 1e-10:
            return True
        else:
            return False

    except OverflowError:
        return False

list_of_coefficients = []

def f10(x):
    return Horner(x, list_of_coefficients)


def userInput():
    global list_of_coefficients

    function_list = "\n".join(f"{key}) {value}" for key, value in zip(functions_map.keys(), functions_strings.values()))
    print(f"Available functions:\n{function_list}")
    chosen_name = input("Choose function: ")

    while chosen_name not in functions_map:
        print("There is no such function. Try again.")
        chosen_name = input("Choose function: ")

    if chosen_name == "f10":
        user_next_numbers = input("Enter the coefficients separated by space: ")
        list_of_coefficients = [float(f) for f in user_next_numbers.split()]
        chosen_function_object = f10
        name_label = "f(x) = "
        for i in range(len(list_of_coefficients)):
            coef = int(list_of_coefficients[i])
            power = len(list_of_coefficients) - 1 - i
            name_label += f" {coef}*x^{power} +"
        name_label = name_label[:-1]
    else:
        chosen_function_object = functions_map[chosen_name]
        name_label = functions_strings[chosen_function_object]

    g_function = lambda x: chosen_function_object(x) * weight_function(x)

    if not check_convergence_symbolic(g_function):
        print(f"\nFunction {name_label} multiplied by weight function tends to infinity.")
        eps_val = "-"
        simpson = "non-convergent"
        gauss = {2: "non-convergent", 3: "non-convergent", 4: "non-convergent", 5: "non-convergent"}

    else:
        eps = float(input("Provide the epsilon value: "))
        while eps <= 0:
            eps = float(input("Epsilon value has to be positive: "))

        eps_val = eps
        simpson = simpson_derivative(g_function, eps)
        gauss = gauss_hermite_quadrature(chosen_function_object)

    table_summary(name_label, eps_val, simpson, gauss)