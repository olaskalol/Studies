import numpy as np

def convergenceCheck(lines_A):
    for i in range (len(lines_A)):
        line = lines_A[i]
        number = line[i]
        sum = 0
        for j in range (len(line)):
            if j == i:
                continue
            sum=sum+abs(line[j])
        if sum > number:
            return False
    return True
def convergenceCheckColumn(lines_A):
    n = len(lines_A)
    for j in range(n):
        number = abs(lines_A[j][j])
        column_sum=0
        for i in range(n):
            if i != j:
                column_sum=column_sum+abs(lines_A[i][j])
        if number <= column_sum:
            return False
    return True

def LDU(lines_A):
    LU = []
    DN1 = []
    for i in range(len(lines_A)):
        line = lines_A[i]
        line_DN1 = []
        line_LU = []
        for j in range (len(line)):
            if j == i:
                line_DN1.append(1/line[j])
                line_LU.append(0)
            else:
                line_LU.append(line[j])
                line_DN1.append(0)
        LU.append(line_LU)
        DN1.append(line_DN1)
    return LU, DN1

def Jacobi(lines_A, lines_b, type_of_stop_condition, value):
    for line in lines_A:
        if len(line)!=len(lines_A):
            print("Error: Invalid Matrix size")
            return 4
    # Kroneckera-Capellego
    A_extended = np.column_stack((lines_A, lines_b))
    A_row = np.linalg.matrix_rank(lines_A)
    A_row_extended = np.linalg.matrix_rank(A_extended)
    if A_row < A_row_extended:
        print("Error: Undetermined system")  # Sprzeczny
        return 1
    elif A_row == A_row_extended and A_row < len(lines_b):
        print("Error: Inconsistent system")  # Nieoznaczony
        return 2

    if convergenceCheck(lines_A)==False and convergenceCheckColumn(lines_A)==False:
        print("The functions are not convergent")
        return 3
    else:
        LU, DN1 = LDU(lines_A)
        LU_array = np.array(LU)
        DN1_array = np.array(DN1)

        M = -DN1_array @ LU_array
        c = DN1_array @ np.array(lines_b)

        result = np.zeros(len(lines_b))
        prev_result = np.zeros(len(lines_b))

        if type_of_stop_condition == "epsilon":
            counter = 1
            prev_result = result
            result = M @ result + c
            while np.max(np.abs(result - prev_result)) >= value :
                prev_result = result
                result = M @ result + c
                counter = counter + 1

        else:
            counter = value
            for i in range(int(value)):
                prev_result = result
                result = M @ result + c

        print(result, prev_result)
        error=np.max(np.abs(result - prev_result))
        return result,counter,error