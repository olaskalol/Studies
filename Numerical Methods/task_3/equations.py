import math

def Horner(x, coefficients):
    summ = coefficients[0]
    for i in range (1, len(coefficients)):
        product = x * summ
        summ = coefficients[i] + product
    return summ

def f1(x):
    return Horner(x, [1,3])
def f2(x):
    return math.sin(x)
def f3(x):
    return math.sin(x) * math.exp(x) - 0.5
def f4(x):
    return x**2 - math.exp(x) + 2
def f5(x):
    return Horner(x, [3,10,3])
def f6(x):
    return (1/3)**(x+3) - 1
def f7(x):
    return x**2 + 5 * math.sin(x - 2)
def f8(x):
    return (1/2)**(2*x**2 + 3*math.cos(x) - 2) - 1/4
def f10(x):
    return
def f9(x):
    return abs(x)
functions_map = {
    "f1": f1,
    "f2": f2,
    "f3": f3,
    "f4": f4,
    "f5": f5,
    "f6": f6,
    "f7": f7,
    "f8": f8,
    "f9": f9,
    "f10": f10,
}

functions_strings = {
    f1: "f(x) = x + 3",
    f2: "f(x) = sin(x)",
    f3: "f(x) = sin(x)e^(x) - 0,5",
    f4: "f(x) = x^2 - e^x + 2",
    f5: "f(x) = 3x^2 + 10x +3",
    f6: "f(x) = (1/3)^(x+3) - 1",
    f7: "f(x) = x^2 + 5sin(x-2)",
    f8: "f(x) = (1/2)^(2x^2 + 3cos(x) - 2) - 1/4",
    f9: "f(x) = |x|",
    f10: "User function"

}

functions_derivatives  = {
    f1: lambda x: 1,
    f2: lambda x: math.cos(x),
    f3: lambda x: math.cos(x) * math.exp(x) + math.sin(x) * math.exp(x),
    f4: lambda x: 2*x - math.exp(x),
    f5: lambda x: 6*x + 10,
    f6: lambda x: (-1) * math.log(3) * (1/3)**(x+3),
    f7: lambda x: 2*x + 5*math.cos(x-2),
    f8: lambda x: -math.log(2) * 2**(-2*x**2 - 3*math.cos(x) + 2) * (4*x - 3*math.sin(x)),
    f9: lambda x: 1 if x > 0 else (-1 if x < 0 else 0)
}