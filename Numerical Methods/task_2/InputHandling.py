def inputHandling(filename):
    list_A=[]
    list_b=[]
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if len(line)<=2:
                list_A.append(line)
                list_b.append(line)
                continue
            line = [float(f) for f in line.split()]
            list_A.append(line[:-1])
            list_b.append(line[-1])
    return list_A, list_b

def handleUserInput():
    manual_or_file= input("Decide if u want to type equations from file or manually (1,2): ")
    while manual_or_file != '1' and manual_or_file != '2':
        manual_or_file= input("Please type equations from file or manually (1,2): ")
    if manual_or_file == "1":
        default_file = "input.txt"
        which_file = input("Write the name of the file (leave empty if default): ").strip()
        if which_file == "":
            which_file = default_file
        lines_A,lines_b= inputHandling(which_file)
        print(lines_A[1])

        rows = input(f"Please type the rows from file you would like to have in matrix (loading every line if empty) ")
        list_of_rows = [int(f) for f in rows.split()]
        inFile=True
        for element in list_of_rows:
            if element > len(lines_A):
                inFile = False
        while not inFile:
            rows = input(
                f"Please type the rows from file you would like to have in matrix (loading every line if empty) ")
            list_of_rows = [int(f) for f in rows.split()]
            inFile = True
            for element in list_of_rows:
                if element >= len(lines_A) or len(lines_A[element-1]) <=2:
                    inFile = False

        if list_of_rows:
            new_lines_A = []
            new_lines_b = []
            for i in range(len(list_of_rows)):
                new_lines_A.append(lines_A[list_of_rows[i]-1])
                new_lines_b.append(lines_b[list_of_rows[i]-1])
            lines_A = new_lines_A
            lines_b = new_lines_b
    else:
        lines_A = []
        lines_b = []
        number_of_eq = int(input("Please type the number of equations: "))
        for i in range(number_of_eq):
            line = input(f"Please type the coefficients of the {i+1}th equation: ")
            list_of_coefficients = [float(f) for f in line.split()]
            lines_A.append(list_of_coefficients[:-1])
            lines_b.append(list_of_coefficients[-1])


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
    return lines_A, lines_b , c , value