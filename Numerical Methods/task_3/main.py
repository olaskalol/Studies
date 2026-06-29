from InputHandling import handleUserInput
from Zad3.Newton_method import correctMatrixMaking
from equations import *
from plot import plot_summary


def f10(x):
    return Horner(x, list_of_coefficients)
# user input

function_list = "\n".join(f"{key}) {value}" for key, value in zip(functions_map.keys(), functions_strings.values()))
print(f"Available functions:\n{function_list}")
chosen_name = input("Choose function: ")

while chosen_name not in functions_map:
    print("There is no such function. Try again.")
    chosen_name = input("Choose function: ")
if chosen_name == "f10":
    user_next_numbers = input("Enter the coefficients separated by space ")
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

a = 0
b = 0
node_matrix = []
while True:
    try:
        user_input = input("Provide the interpolation range: ").split()
        if len(user_input) != 2:
            print("You need to enter exactly two values separated by a space.")
            continue
        a = float(user_input[0])
        b = float(user_input[1])
        if a >= b:
            print("'a' has to be smaller than 'b'")
            continue
        break
    except ValueError:
        print("Please enter valid numbers.")
argument_matrix = handleUserInput()
matrix = correctMatrixMaking(argument_matrix, chosen_function_object)
plot_summary(matrix, a,b, name_label,chosen_function_object)
