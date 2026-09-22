import ctypes
import pandas as pd
import torch
import torch.nn as nn
from pathlib import Path

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

sciantix_options = [
    1,   # 0  iGrainGrowth
    1,   # 1  iFissionGasDiffusivity
    1,   # 2  iDiffusionSolver
    1,   # 3  iIntraGranularBubbleBehavior
    1,   # 4  iResolutionRate
    1,   # 5  iTrappingRate
    1,   # 6  iNucleationRate
    1,   # 7  iOutput
    1,   # 8  iGrainBoundaryVacancyDiffusivity
    1,   # 9  iGrainBoundaryBehaviour
    1,   # 10 iGrainBoundaryMicroCracking
    0,   # 11 iFuelMatrix
    0,   # 12 iGrainBoundaryVenting
    0,   # 13 iRadioactiveFissionGas
    0,   # 14 iHelium
    0,   # 15 iHeDiffusivity
    0,   # 16 iGrainBoundarySweeping
    0,   # 17 iHighBurnupStructureFormation
    0,   # 18 iHighBurnupStructurePorosity
    0,   # 19 iHeliumProductionRate
    0,   # 20 iStoichiometryDeviation
    0,   # 21 iBubbleDiffusivity
    0,   # 22 iChromiumSolubility
    0,   # 23 iDensification
    0,   # 24 iReleaseMode
]

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

temperature_input = (ctypes.c_double * 1)(1550.0)
fissionrate_input = (ctypes.c_double * 1)(1e19)
stress_input = (ctypes.c_double * 1)(0.0)
steam_input = (ctypes.c_double * 1)(0.0)

lib.init_sciantix(
    history, variables, diffusion_modes,
    temperature_input, 1,
    fissionrate_input, 1,
    stress_input, 1,
    steam_input, 1,
)

index_to_name = {
    0: "Grain radius (m)", 1: "Xe produced (at/m3)", 2: "Xe in grain (at/m3)",
    3: "Xe in intragranular solution (at/m3)", 4: "Xe in intragranular bubbles (at/m3)",
    5: "Xe at grain boundary (at/m3)", 6: "Xe released (at/m3)",
    7: "Kr produced (at/m3)", 8: "Kr in grain (at/m3)",
    9: "Kr in intragranular solution (at/m3)", 10: "Kr in intragranular bubbles (at/m3)",
    11: "Kr at grain boundary (at/m3)", 12: "Kr released (at/m3)",
    19: "Intragranular bubble concentration (bub/m3)", 20: "Intragranular bubble radius (m)",
    24: "Intragranular gas bubble swelling (/)",
    25: "Intergranular bubble concentration (bub/m2)", 29: "Intergranular atoms per bubble (at/bub)",
    30: "Intergranular vacancies per bubble (vac/bub)", 31: "Intergranular bubble radius (m)",
    32: "Intergranular bubble area (m2)", 33: "Intergranular bubble volume (m3)",
    34: "Intergranular fractional coverage (/)", 35: "Intergranular saturation fractional coverage (/)",
    36: "Intergranular gas swelling (/)", 37: "Intergranular fractional intactness (/)",
    38: "Burnup (MWd/kgUO2)", 42: "U235 (at/m3)", 45: "U238 (at/m3)",
}

N_STEPS = 50000
records = {name: [] for name in index_to_name.values()}
records["Time (h)"] = []

for idx, name in index_to_name.items():
    records[name].append(variables[idx])
records["Time (h)"].append(0)

for i in range(N_STEPS):
    history[6] = 3600
    history[7] = i
    history[8] = i
    lib.callSciantix(options, history, variables, scaling_factors, diffusion_modes)

    for idx, name in index_to_name.items():
        records[name].append(variables[idx])
    records["Time (h)"].append(i + 1)

python_df = pd.DataFrame(records)

print("Python Xe released at t=50000h:", python_df["Xe released (at/m3)"].iloc[-1])

# SCIANTIX output file
script_dir = Path(__file__).parent 
file_path = script_dir / 'output_1550K.txt'

sciantix_df = pd.read_csv(file_path, sep="\t")
sciantix_df = sciantix_df.loc[:, ~sciantix_df.columns.str.contains("^Unnamed")]

# Check for the same parameters
common_cols = [c for c in python_df.columns if c in sciantix_df.columns and c != "Time (h)"]

merged_py = python_df.set_index("Time (h)")[common_cols]
merged_sc = sciantix_df.set_index("Time (h)")[common_cols]


# Ensures shared rows and columns are the only ones used
merged_py, merged_sc = merged_py.align(merged_sc, join="inner")

# Create python script and SCIANTIX tensors
py_tensor = torch.tensor(merged_py.values, dtype=torch.float64)
sc_tensor = torch.tensor(merged_sc.values, dtype=torch.float64)

threshold = 1e-6 

abs_err = (py_tensor - sc_tensor).abs()

# relative error only where ground truth (SCIANTIX output) is meaningfully large
safe_mask = sc_tensor.abs() > threshold
rel_err = torch.where(safe_mask, abs_err / sc_tensor.abs().clamp(min=threshold), torch.zeros_like(abs_err))

# Compute mean squared error
criterion = nn.MSELoss(reduction='none')
mse_abs = criterion(py_tensor, sc_tensor).mean(dim=0)
mse_rel = (rel_err ** 2).mean(dim=0)

report = pd.DataFrame({
    "mse_abs": mse_abs.numpy(),
    "rmse_abs": mse_abs.sqrt().numpy(),
    "mean_rel_err": rel_err.mean(dim=0).numpy(),
    "max_rel_err": rel_err.max(dim=0).values.numpy(),
}, index=common_cols)

print(report.sort_values("max_rel_err", ascending=False))
report.to_csv('testC_output.csv')