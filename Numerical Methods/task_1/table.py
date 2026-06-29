import matplotlib.pyplot as plt
from root_finding import bisection, secant_method, secant_error

def table_summary(function, start_of_range, end_of_range, label, type_of_stop_condition, value):
    result_bisection = bisection(start_of_range, end_of_range, function, type_of_stop_condition, value)
    result_secant = secant_method(start_of_range, end_of_range, function, type_of_stop_condition, value)

    fig, ax = plt.subplots(figsize=(10, 2))
    ax.axis('tight')
    ax.axis('off')

    range_text = f"[{start_of_range}, {end_of_range}]"
    if type_of_stop_condition == "epsilon":
        criteria_text = f"Epsilon: {value}"
    else:
        criteria_text = f"Max iter: {value}"

    def prepare_row(method_name, result):
        if result is None:
            return [method_name, range_text, criteria_text, "Not found", "-", "-"]
        else:
            x0, iterations, error = result
            error_text = f"{error:.2e}" if isinstance(error, float) else str(error)
            return [method_name, range_text, criteria_text, f"{x0:.6f}", str(iterations), error_text]

    row_bisection = prepare_row("Bisection", result_bisection)
    row_secant = prepare_row("Secant", result_secant)
    rows = [row_bisection, row_secant]
    columns = ["Method", "Range", "Criteria of stop", "Point of zero", "Iterations", "Error"]

    table = ax.table(
        cellText = rows,
        colLabels = columns,
        loc = 'center',
        cellLoc = 'center'
    )
    table.auto_set_font_size(False)
    table.set_fontsize(12)
    table.scale(1, 2)
    for (row, col), cell in table.get_celld().items():
        if row == 0:
            cell.set_text_props(weight='bold')

    plt.tight_layout()
    plt.show()
