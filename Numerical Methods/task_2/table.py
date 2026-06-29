import matplotlib.pyplot as plt
from Jacobi import Jacobi


def table_summary( lines_A, lines_b ,type_of_stop_condition, value):
    result_Jacobi = Jacobi(lines_A, lines_b, type_of_stop_condition, value)
    if type_of_stop_condition == "epsilon":
        criteria_text = f"Epsilon: {value}"
    else:
        criteria_text = f"Max iter: {value}"

    fig, ax = plt.subplots(figsize=(10, 4))
    ax.axis('off')

    def prepare_row( result):
        if result_Jacobi == 1:
            return [criteria_text , "Undetermined system", "-", "-"]
        elif result_Jacobi == 2:
            return [criteria_text , "Inconsistent system", "-", "-"]
        elif result_Jacobi == 3:
            return [ criteria_text , "Not convergent", "-", "-"]
        elif result_Jacobi == 4:
            return [criteria_text , "Invalid Matrix size", "-", "-"]
        else:
            solutions, iterations, error = result
            solutions_str = "\n".join([f"{x:.7f}" for x in solutions])
            error_text = f"{error:.7f}"
            return [ criteria_text, solutions_str, str(iterations), error_text]
    rows = [prepare_row(result_Jacobi)]
    columns = [ "Criteria of stop", "Solutions", "Iterations", "Error"]

    table = ax.table(
        cellText = rows,
        colLabels = columns,
        loc = 'center',
        cellLoc = 'center'
    )
    table.auto_set_font_size(False)
    table.set_fontsize(10)
    table.scale(1.2, 2.5)
    for (row, col), cell in table.get_celld().items():
        if row == 0:
            cell.set_text_props(weight='bold')

    plt.tight_layout()
    plt.show()
