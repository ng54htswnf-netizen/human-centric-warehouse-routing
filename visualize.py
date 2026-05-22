import matplotlib.pyplot as plt
import numpy as np

# 1. Load the data from your C++ program
data = np.loadtxt('path_data.csv', delimiter=',')

# 2. Define a custom color map
# 0=Floor(White), 1=Shelf(Black), 2=Bin(Red), 3=Path(Blue)
cmap = plt.cm.colors.ListedColormap(['white', 'black', 'red', 'blue'])

plt.figure(figsize=(8, 8))
plt.imshow(data, cmap=cmap)

# 3. Add a grid and labels
plt.title(" Warehouse - Human-Centric Path Optimization")
plt.grid(which='both', color='gray', linestyle='-', linewidth=0.5)
plt.xticks(range(10))
plt.yticks(range(10))

plt.show()