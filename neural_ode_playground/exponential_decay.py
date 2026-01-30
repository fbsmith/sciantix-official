import numpy as np
from sklearn.preprocessing import StandardScaler
from sklearn.neural_network import MLPRegressor
import matplotlib.pyplot as plt

# problem constants
n_0 = 1.0
T_half = 32904.72
lambda_n = np.log(2) / T_half

# time grid
t_max = T_half * 6
n_t = 10_000
t = np.linspace(0.0, t_max, n_t)

def dn_dt(n):
    return - lambda_n * n

def exact(t):
    return n_0 * np.exp(-lambda_n * t)

def euler(t):
    dt = t[1] - t[0]

    # number grid
    n = np.zeros(n_t)
    n[0] = n_0

    for i in range(1, n_t):
        n[i] = n[i-1] + dt * dn_dt(n[i-1])
    
    return n

n_exact = exact(t)
dn_exact = dn_dt(n_exact)
n_euler = euler(t)

nn_model = MLPRegressor(hidden_layer_sizes=(100, 10), max_iter = 1000)
features = np.zeros((n_t, 2))
for i in range(n_t):
    features[i, 0] = t[i]
    features[i, 1] = n_0
scaler = StandardScaler()
features = scaler.fit_transform(features)
nn_model.fit(features, n_exact)
n_nn_model = nn_model.predict(features)

# plotting
# plt.subplot(1, 2, 1)
plt.plot(t / 3600, n_exact, label = "exact")
plt.plot(t / 3600, n_euler, label = "euler")
plt.plot(t / 3600, n_nn_model, label = "NN Model")
plt.xlabel("Time (hr)")
plt.ylabel("Number (atoms)")
plt.legend()
plt.grid()

# plt.subplot(1, 2, 2)
# plt.plot(t / 3600, dn_exact / 3600, label = "exact")
# plt.xlabel("Time (hr)")
# plt.ylabel("Rate of Decay (atoms/hr)")
# plt.legend()
# plt.grid()

plt.suptitle("Xe-135 Decay over Time")
plt.tight_layout()
plt.show()
