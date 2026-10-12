import numpy as np
import UQpy
import math
import os
import matplotlib.pyplot as plt

def example_model(x, **kwargs):
    return np.sin(x[:,0]) + 7 * np.sin(x[:,1])**2 + 0.1 * x[:,2]**4 * np.sin(x[:,0]) + x[:,3]

py_model = UQpy.PythonModel(
    model_script=os.path.basename(__file__), 
    model_object_name='example_model', 
    model_object=example_model, 
    var_names=['x1', 'x2', 'x3', 'x4']
)
model = UQpy.RunModel(model=py_model)

param1 = UQpy.Uniform(loc=0, scale=math.pi)
param2 = UQpy.Uniform(loc=0, scale=math.pi)
param3 = UQpy.Uniform(loc=0, scale=math.pi)
param4 = UQpy.Uniform(loc=0, scale=math.pi)

joint_dist = UQpy.JointIndependent([param1, param2, param3, param4])

morris = UQpy.MorrisSensitivity(runmodel_object=model, distributions=joint_dist,
                                n_levels=6, maximize_dispersion=True)

morris.run(n_trajectories=10)

print("Mu* indices:", morris.mustar_indices)
print("Sigma indices", morris.sigma_indices)

plt.figure()

labels = ['$x_1$', '$x_2$', '$x_3$', '$x_4$']
colors = ['blue', 'orange', 'green', 'red']

# Loop through each parameter index to plot them as distinct legend items
for i in range(len(labels)):
    plt.scatter(
        morris.mustar_indices[i], 
        morris.sigma_indices[i], 
        c=colors[i], 
        label=labels[i],
        s=50
    )

# Formatting the plot
plt.xlabel("$\mu^*$ (Mean of Absolute Elementary Effects)")
plt.ylabel("$\sigma$ (Standard Deviation of Elementary Effects)")
plt.title('Ishigami Function: $\sin (x_1) + a\sin^2 (x_2) + bx_3^4 \sin (x_1) + x_4$')
plt.grid(True, linestyle='--', alpha=0.5)

plt.legend() 
plt.show()
