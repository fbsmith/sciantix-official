"""
This script processes raw text files of data produced by sciantix
"""
import pandas as pd
import numpy as np
import torch
import torch.nn as nn
from torchdiffeq import odeint
import matplotlib.pyplot as plt

path = r"~/projects/sciantix-official/neural_ode_playground/output.txt"
try:
    df = pd.read_csv(path, delimiter = "\t", dtype = float)
except FileNotFoundError:
    raise FileNotFoundError("Could not find file.")

# clean up dataframe
df.drop(columns = ["Unnamed: 34"], inplace = True)

# convert to numpy
cols = df.columns
col_index = {}
for i in range(len(cols)):
    col_index[cols[i]] = i
data = df.to_numpy(dtype = np.float32)

# convert to torch.tensor
features = torch.from_numpy(data)
init_fgr = features[0, col_index["Fission gas release (/)"]].unsqueeze(0)
times = features[:, col_index["Time (h)"]]
fgr = features[:, col_index["Fission gas release (/)"]]

# Neural ODE definition
class ODEFunc(nn.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(1, 500),
            nn.Tanh(),
            nn.Linear(500, 1),
            nn.Tanh(),
        )
    
    def forward(self, t, states):
        return self.net(states)

odefunc = ODEFunc()

# Optimizer
optimizer = torch.optim.Adam(odefunc.parameters(), lr=1e-3)

for epoch in range(10):
    print("Start training")
    # pred = odeint(odefunc, init_fgr, times)
    # fgr.unsqueeze(1)
    # loss = torch.mean((pred - fgr)**2)

    # optimizer.zero_grad()
    # loss.backward()
    # optimizer.step()

    # if epoch % 100 == 0:
    #     print(f"Epoch {epoch} loss {loss.item():.4f}")
    print("End training")

plt.plot(times, fgr, label = "Sciantix")
plt.title(f"Fission Gas Release vs Time\nTemp = {df["Temperature (K)"][0]} K")
plt.xlabel("Time (h)")
plt.ylabel("Fission gas release (/)")
plt.grid()
plt.legend()
plt.show()
