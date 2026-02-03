import torch
import torch.nn as nn
from torchdiffeq import odeint
import matplotlib.pyplot as plt

class ODEFunc(nn.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(2, 64),
            nn.Tanh(),
            nn.Linear(64, 2),
        )
    
    def forward(self, t, y):
        return self.net(y)

f = ODEFunc()

y0 = torch.randn(10, 2)
t = torch.linspace(0, 1, 100)

y = odeint(f, y0, t, method="dopri5")

plt.plot(t, y)
plt.show()
