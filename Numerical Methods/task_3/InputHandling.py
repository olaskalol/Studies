def inputHandling(filename):
    nodes = []
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            nodes.append(float(line))
    return nodes


def handleUserInput():
    nodes = []
    all_nodes = []

    manual_or_file = input("Do you want to provide arguments in a file or by terminal? (1, 2): ")
    while manual_or_file not in ['1', '2']:
        manual_or_file = input("Please provide arguments by terminal or in a file (1,2): ")

    if manual_or_file == "1":
        default_file = "input.txt"
        which_file = input("Write the name of the file (leave empty if default): ").strip()
        if which_file == "":
            which_file = default_file

        all_nodes = inputHandling(which_file)
        nodes = all_nodes.copy()

        while True:
            rows = input("Please type the rows from file with your desired arguments (loading every line if empty): ")
            if not rows.strip():
                break

            try:
                list_of_rows = [int(f) for f in rows.split()]
            except ValueError:
                print("Please enter valid integers separated by space.")
                continue

            if any(element > len(all_nodes) for element in list_of_rows):
                print(f"Error: Some rows exceed the file size ({len(all_nodes)}). Try again.")
                continue

            nodes = [all_nodes[i - 1] for i in list_of_rows]
            break

    else:
        while True:
            try:
                number_of_eq = int(input("Please type the number of arguments: "))
                if number_of_eq <= 0:
                    print("Number of arguments must be greater than 0.")
                    continue
                break
            except ValueError:
                print("Please enter a valid integer.")

        for i in range(number_of_eq):
            while True:
                try:
                    line = input(f"Please type the {i + 1}th argument: ")
                    nodes.append(float(line))
                    break
                except ValueError:
                    print("Please enter a valid single number.")

    diff = []
    for i in range(len(nodes) - 1):
        diff.append(nodes[i + 1] - nodes[i])

    while len(set(diff)) == 1 and len(diff) > 1:
        print("The intervals between arguments should be uneven. Please provide new nodes")

        if manual_or_file == "1":
            while True:
                rows = input(
                    "Please type the rows from file with your desired arguments (loading every line if empty): ")
                if not rows.strip():
                    nodes = all_nodes.copy()
                    break

                try:
                    list_of_rows = [int(f) for f in rows.split()]
                except ValueError:
                    print("Please enter valid integers separated by space.")
                    continue

                if any(element > len(all_nodes) for element in list_of_rows):
                    print(f"Error: Some rows exceed the file size ({len(all_nodes)}). Try again.")
                    continue

                nodes = [all_nodes[i - 1] for i in list_of_rows]
                break

        else:
            nodes = []
            for i in range(number_of_eq):
                while True:
                    try:
                        line = input(f"Please type the {i + 1}th argument: ")
                        nodes.append(float(line))
                        break
                    except ValueError:
                        print("Please enter a valid single number.")

        diff = []
        for i in range(len(nodes) - 1):
            diff.append(nodes[i + 1] - nodes[i])

    return nodes