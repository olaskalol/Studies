import matplotlib.pyplot as plt
import textwrap


def table_summary_approx(function_name, degree, nodes, error_val, mode_name):
    fig, ax = plt.subplots(figsize=(12, 2.5))
    ax.axis('off')

    columns = ["Funkcja", "Tryb pracy", "Stopień wielomianu", "Liczba węzłów", "Błąd aproksymacji"]

    wrapped_function_name = textwrap.fill(function_name, width=25)
    if isinstance(error_val, (int, float)):
        formatted_error = f"{error_val:.6e}"
    else:
        formatted_error = str(error_val)

    row_data = [
        wrapped_function_name,
        mode_name,
        str(degree),
        str(nodes),
        formatted_error
    ]

    szerokosci_kolumn = [0.30, 0.10, 0.20, 0.20, 0.20]

    table = ax.table(cellText=[row_data], colLabels=columns, colWidths=szerokosci_kolumn,
                     loc='center', cellLoc='center')

    table.auto_set_font_size(False)
    table.set_fontsize(11)
    table.scale(1.0, 3.5)

    for (row, col), cell in table.get_celld().items():
        if row == 0:
            cell.set_text_props(weight='bold', color='white')
            cell.set_facecolor('#40466e')
        else:
            cell.set_facecolor('#f1f1f2')

            if col == 4:
                if isinstance(error_val, (int, float)) and error_val > 1.0:
                    cell.get_text().set_color('red')
                elif isinstance(error_val, str) and error_val != "-":
                    cell.get_text().set_color('red')

    plt.tight_layout()
    plt.show()