import time
import ctypes
import pandas as pd
import torch
import torch.nn as nn
import numpy as np
import UQpy
import os
import matplotlib.pyplot as plt

lib = ctypes.CDLL("./sciantix-official/build/sciantix.dll")

# Declaring true argument signatures

# void getSciantixOptions(int Sciantix_options[], double Sciantix_scaling_factors[]);
lib.getSciantixOptions.argtypes = [
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_double),
]
lib.getSciantixOptions.restype = None

# void callSciantix(int options[], double history[], double variables[],
#                    double scaling_factors[], double diffusion_modes[]);
lib.callSciantix.argtypes = [
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double),
]
lib.callSciantix.restype = None

# void Initialization(double history[], double variable[], double diffusion_modes[],
#                      temperate[], fissionrate[], hydrostress[], pressure[]);
lib.init_sciantix.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # Sciantix_history
    ctypes.POINTER(ctypes.c_double),  # Sciantix_variables
    ctypes.POINTER(ctypes.c_double),  # Sciantix_diffusion_modes
    ctypes.POINTER(ctypes.c_double), ctypes.c_int,  # Temperature_input, len
    ctypes.POINTER(ctypes.c_double), ctypes.c_int,  # Fissionrate_input, len
    ctypes.POINTER(ctypes.c_double), ctypes.c_int,  # Hydrostaticstress_input, len
    ctypes.POINTER(ctypes.c_double), ctypes.c_int,  # Steampressure_input, len
]
lib.init_sciantix.restype = None

sciantix_options = [1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
N = 100
deltaT = 3600

def run_sciantix(inputs):
    temperature, fission_rates, stress = inputs

    sciantix_variables = [0.0] * 161
    sciantix_variables[0]   = 5.0e-06   # Grain radius, m
    sciantix_variables[40]  = 10641.0   # Fuel density, kg/m3
    sciantix_variables[42]  = 3.0       # U235, at/m3
    sciantix_variables[45]  = 97.0      # U238, at/m3

    options = (ctypes.c_int * 25)(*sciantix_options)
    scaling_factors = (ctypes.c_double * 9)(*[1.0]*9)
    variables = (ctypes.c_double * 161)(*sciantix_variables)
    diffusion_modes = (ctypes.c_double * 720)(*[0.0]*720)
    history = (ctypes.c_double * 11)()   # will be overwritten each step

    temperature_input = (ctypes.c_double * 1)(temperature)
    fission_rate_input = (ctypes.c_double * 1)(fission_rates)
    stress_input = (ctypes.c_double * 1)(stress)
    steam_input = (ctypes.c_double * 1)(0.0)

    lib.init_sciantix(
        history, variables, diffusion_modes,
        temperature_input, 1,
        fission_rate_input, 1,
        stress_input, 1,
        steam_input, 1,
        )

    for i in range(N):
        history[6] = 3600
        history[7] = i
        history[8] = i
        lib.callSciantix(options, history, variables, scaling_factors, diffusion_modes)
    return variables[34] #Intergranular fractional coverage

def sciantix_model(x, **kwargs):
    outputs = np.zeros(x.shape[0])
    for i, row in enumerate(x):
        outputs[i] = run_sciantix(row)
    return outputs

py_model = UQpy.PythonModel(
    model_script="sciantix_morris_testing.py", 
    model_object_name='sciantix_model', 
    model_object=sciantix_model, 
    var_names=['temperature','fission_rate','stress']
)
model = UQpy.RunModel(model=py_model)

param_temp = UQpy.Uniform(loc=1273, scale=200)
param_fission_rate = UQpy.Uniform(loc=1e19, scale=2e19)
param_stress = UQpy.Uniform(loc=50, scale=100)

levels = [4, 6, 8, 10, 12, 14, 16, 18, 20]
mu_star = []
sigma = []

for n_lev in levels:
    distribution = UQpy.JointIndependent([param_temp, param_fission_rate, param_stress])
    morris = UQpy.MorrisSensitivity(runmodel_object=model, distributions=distribution,
                                n_levels=n_lev, maximize_dispersion=True,)
    t0 = time.time()
    morris.run(n_trajectories=25)
    t1 = time.time()
    print(f"Morris run took {t1-t0:.1f} s")

    print("Mu* indices:", morris.mustar_indices)
    print("Sigma indices:", morris.sigma_indices)
    mu_star.append(np.array(morris.mustar_indices))
    sigma.append(np.array(morris.sigma_indices))

mu_star = np.vstack(mu_star)
sigma = np.vstack(sigma)

print("Mu* array: ", mu_star)
print("Sigma indices: ", sigma)

mu_star_temp = mu_star[:,0]
mu_star_fr = mu_star[:,1]
mu_star_stress = mu_star[:,2]

sigma_temp = sigma[:,0]
sigma_fr = sigma[:,1]
sigma_stress = sigma[:,2]

plt.figure()
#plt.scatter(mu_star, sigma, c=['blue', 'orange', 'green']) # Blue = Temp., Orange = FR, Green = Stress
plt.plot(levels, mu_star_temp, marker = 'o')
plt.plot(levels, mu_star_fr, marker = 'o')
plt.plot(levels, mu_star_stress, marker = 'o')
plt.xlabel("n-levels")
plt.ylabel("Mu*")
plt.title("Morris Screening Results")
plt.savefig("mustar_plot_fractional_cvg.png")

plt.figure()
#plt.scatter(mu_star, sigma, c=['blue', 'orange', 'green']) # Blue = Temp., Orange = FR, Green = Stress
plt.plot(levels, sigma_temp, marker = 'o')
plt.plot(levels, sigma_fr, marker = 'o')
plt.plot(levels, sigma_stress, marker = 'o')
plt.xlabel("n-levels")
plt.ylabel("Sigma")
plt.title("Morris Screening Results")
plt.savefig("sigma_plot_fractional_cvg.png")

print("Saved plot .png")