import matplotlib.pyplot as plt
import textwrap

def table_summary(function_name, eps, simpson_result, gauss_results):
    fig, ax = plt.subplots(figsize=(12, 3))
    ax.axis('off')

    columns = ["Function", "Epsilon", "Simpson", "Gauss (n=2)", "Gauss (n=3)", "Gauss (n=4)", "Gauss (n=5)"]

    def format_val(val):
        if isinstance(val, str):
            return val
        return f"{val:.6f}"

    wrapped_function_name = textwrap.fill(function_name, width=25)

    row_data = [
        wrapped_function_name,
        format_val(eps),
        format_val(simpson_result),
        format_val(gauss_results.get(2, "non-convergent")),
        format_val(gauss_results.get(3, "non-convergent")),
        format_val(gauss_results.get(4, "non-convergent")),
        format_val(gauss_results.get(5, "non-convergent"))
    ]

    szerokosci_kolumn = [0.25, 0.10, 0.13, 0.13, 0.13, 0.13, 0.13]
    table = ax.table(cellText=[row_data], colLabels=columns, colWidths=szerokosci_kolumn, loc='center',
                     cellLoc='center')

    table.auto_set_font_size(False)
    table.set_fontsize(11)
    table.scale(1.0, 3.5)

    for (row, col), cell in table.get_celld().items():
        if row == 0:
            cell.set_text_props(weight='bold', color='white')
            cell.set_facecolor('#40466e')
        else:
            cell.set_facecolor('#f1f1f2')
            if "non-convergent" in cell.get_text().get_text():
                cell.get_text().set_color('red')

    plt.tight_layout()
    plt.show()