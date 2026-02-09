import torch
import torch.nn as nn
import matplotlib.pyplot as plt
from torchdiffeq import odeint

# Generate noisy circular data
def true_dynamics(t, states):
    x, y = states
    return torch.stack([-y, x])

t = torch.linspace(0., 10., steps=100)
init = torch.tensor([1., 0.])
true_traj = odeint(true_dynamics, init, t)
noisy_traj = true_traj + 0.1 * torch.randn_like(true_traj)

# Neural ODE definition
class ODEFunc(nn.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(2, 500),
            nn.Tanh(),
<<<<<<< HEAD
=======
            nn.Linear(500, 500),
            nn.Tanh(),
>>>>>>> 54ca0f2 (Leveraged ChatGPT to get a working NeuralODE)
            nn.Linear(500, 2),
        )
    
    def forward(self, t, states):
        return self.net(states)
    
odefunc = ODEFunc()

# Optimizer
optimizer = torch.optim.Adam(odefunc.parameters(), lr=1e-3)

for epoch in range(2000):
    pred = odeint(odefunc, init, t)
    loss = torch.mean((pred - noisy_traj)**2)

    optimizer.zero_grad()
    loss.backward()
    optimizer.step()

    if epoch % 100 == 0:
        print(f"Epoch {epoch} loss {loss.item():.4f}")

# Plot results
pred = odeint(odefunc, init, t).detach()
plt.plot(true_traj[:, 0], true_traj[:, 1], '.', label = 'True Data')
plt.plot(noisy_traj[:, 0], noisy_traj[:, 1], '.', label = 'NoisyData')
plt.plot(pred[:, 0], pred[:, 1], '-', label = 'Learned')
plt.legend()
plt.savefig("chat_gpt_example.png")
plt.show()
