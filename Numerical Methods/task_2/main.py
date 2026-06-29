from InputHandling import handleUserInput
from Jacobi import LDU, Jacobi
from table import table_summary

A, b, c, value = handleUserInput()
print(table_summary(A, b, c, value))