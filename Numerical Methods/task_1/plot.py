import pandas as pd
from root_finding import bisection, secant_method
from matplotlib import pyplot as plt
from numpy import linspace, vectorize

def plot_summary(function, start_of_range, end_of_range, label, type_of_stop_condition, value):
   #result of bisection from data chose by user
    result_bisection = bisection(start_of_range, end_of_range, function, type_of_stop_condition, value)
    result_secant = secant_method(start_of_range, end_of_range, function, type_of_stop_condition, value)

    #visualizing function for computer like it's data table
    x_vec = linspace(start_of_range, end_of_range, 500) #num is used to decide how many "dots" will be drawn
    y_vec = vectorize(function)(x_vec) #makes list of points to draw
    df = pd.DataFrame({'x': x_vec, 'y': y_vec}) #data table

    #drawing function
    plt.figure(figsize=(12, 8))
    plt.plot(df['x'], df['y'], label=label, color='pink', linewidth=2)
    error= None
    #if bisection worked the result of it is also shown
    if result_bisection:
        root, iterations ,error= result_bisection
        #root and function(root) is result of bisection as function's possible zero
        plt.scatter(root, function(root), color='red', s=30, zorder=3,
                    label=f'Miejsce zerowe: {root:.5f}\n(iteracje: {iterations})')
        #showing info right next to dot
        plt.annotate(f'  x0 ≈ {root:.5f}', (root, function(root)), fontsize=12, color='red')
        # if bisection worked the result of it is also shown

   # if secant worked the result of it is also shown
    if result_secant:
        root, iterations, error = result_secant
        # root and function(root) is result of bisection as function's possible zero
        plt.scatter(root, function(root), color='blue', s=30, zorder=3, marker="x",
                    label=f'Miejsce zerowe: {root:.5f}\n(iteracje: {iterations})')
        # showing info right next to dot
        plt.annotate(f'  x0 ≈ {root:.5f}', (root, function(root)-0.5), fontsize=12, color='blue')

    plt.axhline(0, color='black', linewidth=1)
    plt.axvline(0, color='black', linewidth=1, alpha=0.3)
    plt.grid(True, linestyle=':', alpha=0.6)
    if type_of_stop_condition == "epsilon":
        name_of_stop_condition = f"epsilon o wartości {value} "
    else:
        name_of_stop_condition = f" {value} iteracji "
    if error is not None:
        plt.title(f"Analiza miejsca zerowego dla {label} z warunkiem stopu {name_of_stop_condition}")
    else:
       plt.title(f"Analiza miejsca zerowego dla {label} z warunkiem stopu {name_of_stop_condition} ")
    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.legend()
    plt.show()
