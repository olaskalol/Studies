from equations import *
from root_finding import bisection, secant_method
from plot import plot_summary
from table import table_summary

def f9(x):
    return Horner(x, list_of_coefficients)

# user input

function_list = "\n".join(f"{key}) {value}" for key, value in zip(functions_map.keys(), functions_strings.values()))
print(f"Dostępne funkcje:\n{function_list}")
chosen_name = input("Choose function: ")

while chosen_name not in functions_map:
    print("There is no such function. Try again.")
    chosen_name = input("Choose function: ")
if chosen_name == "f9":
    user_next_numbers = input("enter the next coefficients separated by a space ")
    list_of_coefficients = [float(f) for f in user_next_numbers.split()]
    chosen_function_object = f9
    name_label = "f(x) = "
    for i in range(len(list_of_coefficients)):
        coef = int(list_of_coefficients[i])
        power = len(list_of_coefficients) - 1 - i
        name_label += f" {coef}*x^{power} +"
    name_label = name_label[:-1]

else:
    list_of_coefficients = None
    chosen_function_object = functions_map[chosen_name]
    name_label = functions_strings[chosen_function_object]

a = 0
b = 0
while True:
    try:
        user_input = input("Choose the numerical range (e.g., '1 5'): ").split()
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

c = (input("Choose the criteria of stoping the algorithm (epsilon, iterations): ")).strip()
while c != "epsilon" and c != "iterations":
    print("Please enter either epsilon or iterations.")
    c = input("Choose the criteria of stoping the algorithm (epsilon, iterations): ")

value = 0
if c == "epsilon":
    while True:
        try:
            value = float(input("Enter the epsilon value: "))
            if value <= 0:
                print("Please enter a positive number.")
                continue
            break
        except ValueError:
            print("Please enter a valid numerical value.")
elif c == "iterations":
    while True:
        try:
            value = float(input("Enter the maximum iterations: "))
            if value <= 0:
                print("Please enter a positive number.")
                continue
            break
        except ValueError:
            print("Please enter a valid integer value.")

print(bisection(a, b, chosen_function_object, c, value))
print(secant_method(a, b, chosen_function_object, c, value, list_of_coefficients))
print(plot_summary(chosen_function_object,a,b,name_label,c,value))
print(table_summary(chosen_function_object,a,b,name_label,c,value))
