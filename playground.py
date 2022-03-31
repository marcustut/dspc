x1 = [7, 6, 8, 9, 14, 19]
x2 = [8, 6, 9, 8, 13, 18]
y = [1000, 1789, 2000, 2400, 3800, 4000]

sum_of_x1 = sum(x1)
sum_of_x2 = sum(x2)
sum_of_y = sum(y)

mean_x1 = sum_of_x1 / len(x1)
mean_x2 = sum_of_x2 / len(x2)
mean_y = sum_of_y / len(y)

sum_of_squares_x1 = sum([(a - mean_x1) ** 2 for a in x1])
sum_of_squares_x2 = sum([(a - mean_x2) ** 2 for a in x2])

sum_of_products_x1_y = sum(
    list(map(lambda a, b: (a - mean_x1) * (b - mean_y), x1, y)))
sum_of_products_x2_y = sum(
    list(map(lambda a, b: (a - mean_x2) * (b - mean_y), x2, y)))
sum_of_products_x1_x2 = sum(
    list(map(lambda a, b: (a - mean_x1) * (b - mean_x2), x1, x2)))

b1 = (sum_of_products_x1_y * sum_of_squares_x2 -
      sum_of_products_x1_x2 * sum_of_products_x2_y) / (sum_of_squares_x1 * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x1_x2)

b2 = (sum_of_products_x2_y * sum_of_squares_x1 -
      sum_of_products_x1_x2 * sum_of_products_x1_y) / (sum_of_squares_x1 * sum_of_squares_x2 - sum_of_products_x1_x2 * sum_of_products_x1_x2)

a = mean_y - (b1 * mean_x1) - (b2 * mean_x2)


def predictY(x1, x2):
    # y = b1x1 + b2x2 + a
    return (b1 * x1) + (b2 * x2) + a


print("Sum of X1:", sum_of_x1)
print("Sum of X2:", sum_of_x2)
print("Sum of Y:", sum_of_y)
print("Mean X1:", mean_x1)
print("Mean X2:", mean_x2)
print("Mean Y:", mean_y)
print("Sum of Squares X1:", sum_of_squares_x1)
print("Sum of Squares X2:", sum_of_squares_x2)
print("Sum of Products X1Y:", sum_of_products_x1_y)
print("Sum of Products X2Y:", sum_of_products_x2_y)
print("Sum of Products X1X2:", sum_of_products_x1_x2)
print("Gradient of X1:", b1)
print("Gradient of X2:", b2)
print("Y-Intercept:", a)

print("Predict for X1 = 9, X2 = 10:", predictY(9, 10))
