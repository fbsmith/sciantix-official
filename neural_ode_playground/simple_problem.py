import numpy as np
import matplotlib.pyplot as plt

# time grid
t_max = 1000.0
n_t = 1000
t = np.linspace(0.0, t_max, n_t)

# concentration grid
c_0 = 1.0
c = np.full(n_t, c_0)

# plotting
plt.figure()
plt.plot(t, c)
plt.title("Concentration over Time")
plt.xlabel("Time (s)")
plt.ylabel("Concentration (mol)")
plt.grid()
plt.show()
