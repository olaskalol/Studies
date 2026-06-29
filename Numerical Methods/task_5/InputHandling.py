from equations import *
from plot import plot_approximation_summary
from Approximation import get_coefficients, calculate_approximation_error, auto_approximate
from table import table_summary_approx


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
        print(f"\nFunction {name_label} non-convergent.")

        table_summary_approx(
            function_name=name_label,
            degree="-",
            nodes="-",
            error_val="Brak zbieżności",
            mode_name="Odrzucono"
        )

    else:
        while True:
            try:
                app_range = input("Provide the approximation range: ").split()
                if len(app_range) != 2:
                    print("You need to enter exactly two values separated by a space.")
                    continue
                a = float(app_range[0])
                b = float(app_range[1])
                if a >= b:
                    print("'a' has to be smaller than 'b'")
                    continue
                break
            except ValueError:
                print("Please enter valid numbers.")

        print("\nChoose mode")
        print("1) Providing degree of a polynomial")
        print("2) Automatic degree chooser depending on a error value")

        mode = input("Your choice (1/2): ")
        while mode not in ["1", "2"]:
            mode = input("Incorrect choice. Please enter 1 or 2: ")

        if mode == "1":
            mode_name = "Ręczny"
            app_deg = int(input("Provide the degree of approximation polynomial: "))
            while app_deg < 0:
                app_deg = int(input("The degree has to be positive: "))

            number_of_nodes = int(input("Enter the number of nodes: "))
            while number_of_nodes <= 0:
                number_of_nodes = int(input("Number of nodes must be positive: "))

            coefficients = get_coefficients(chosen_function_object, app_deg, number_of_nodes)
            error_val = calculate_approximation_error(chosen_function_object, coefficients, number_of_nodes)
            final_deg = app_deg

        else:
            mode_name = "Auto"
            expected_error = float(input("Type expected error value (e.g. 0.01): "))
            number_of_nodes = int(input("Enter the base number of nodes (np. 5): "))
            while number_of_nodes <= 0:
                number_of_nodes = int(input("Number of nodes must be positive: "))
            final_deg, coefficients, error_val = auto_approximate(chosen_function_object, expected_error, max_deg=15, n_nodes=number_of_nodes)

        table_summary_approx(name_label, final_deg, number_of_nodes, error_val, mode_name)
        plot_approximation_summary(a, b, name_label, chosen_function_object, coefficients, error_val, final_deg)