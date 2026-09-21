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
N = 50000
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
    return variables[6] #Xe produced, Fission gas release

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

distribution = UQpy.JointIndependent([param_temp, param_fission_rate, param_stress])
morris = UQpy.MorrisSensitivity(runmodel_object=model, distributions=distribution,
                                n_levels=4, maximize_dispersion=True,)

t0 = time.time()
morris.run(n_trajectories=20)
t1 = time.time()
print(f"Morris run took {t1-t0:.1f} s")

print("Mu* indices:", morris.mustar_indices)
print("Sigma indices:", morris.sigma_indices)

plt.figure()
plt.scatter(morris.sigma_indices, morris.mustar_indices, c=['blue', 'orange', 'green'])
plt.xlabel("Sigma")
plt.ylabel("Mu*")
plt.title("Morris Screening Results")
plt.savefig("morris_plot_xerelease.png")
print("Saved plot .png")