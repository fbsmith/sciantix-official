import re
import time
import ctypes
import importlib
import numpy as np
import pandas as pd
import UQpy
import os
import matplotlib.pyplot as plt

N_SAMPLES = 1024      # Sample size
SOURCE_FILE = "./sciantix-official/src/operations/UpdateVariables.C"   # holds SCIANTIX's variable names

RANGES = {
    "temperature":  (1273.0, 1673.0),
    "fission_rate": (1e19, 3e19),
    "stress":       (0.0, 100.0),
}
input_names = list(RANGES)

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

# Recording inputs and outputs from each run
INPUTS, OUTPUTS = [], [] 

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
    history = (ctypes.c_double * 11)()

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
    return np.array(variables, dtype=float)     # Returns an array of all SCIANTIX variables

# Create model to be used for UQpy
def sciantix_model(x, **kwargs):
    outputs = []    # Collects results for inputs
    for row in x:
        output = run_sciantix(row)
        INPUTS.append(row.copy())       # Saves row inputs globally ([T,F,S])
        OUTPUTS.append(output)          # Saves row outputs globally ([all 161 variables])
        if len(OUTPUTS) <= 4:
            print(f"run {len(OUTPUTS)}: inputs = {row} -> Xe produced = {output[1]:.4g}, Xe released = {output[6]:.4g}")
        outputs.append(output)      # Save outputs locally
    return outputs[0]    # Returns a (1,161) vector

# Retrieving output variable names from UpdateVariables.C
def load_variable_names(path):
    text = open(path, encoding="utf-8", errors="ignore").read()
    # Finds all instances of the following format: {0, "Grain radius"}, to build a
    # dictionary that maps each output index to its name
    return {int(index): name for index, name in re.findall(r'\{\s*(\d+)\s*,\s*"([^"]+)"\s*\}', text)}

# UQpy imports a copy of the script as a module, if __name__... skips the copied code so
# the code below isn't ran again
if __name__ == "__main__":

    # =================== Running Sobol ===================
    names = load_variable_names(SOURCE_FILE)

    # Calling sciantix_model so UQpy can use it
    py_model = UQpy.PythonModel(
        model_script=os.path.basename(__file__), 
        model_object_name='sciantix_model', 
        model_object=sciantix_model, 
        var_names=input_names
    )
    model = UQpy.RunModel(model=py_model)

    # Creating input distribution
    distribution = UQpy.JointIndependent([
        UQpy.Uniform(loc=1273.0, scale=400.0),
        UQpy.Uniform(loc=1e19,   scale=2e19),
        UQpy.Uniform(loc=0.0,    scale=100.0),
    ])

    # Imports script so model inputs and outputs can be read
    sm = importlib.import_module(os.path.splitext(os.path.basename(__file__))[0])

    # Performing Sobol indices analysis
    sobol = UQpy.SobolSensitivity(model, distribution)
    t0 = time.time()
    sobol.run(n_samples = N_SAMPLES)

    # UQpy creates N x (d+2) rows of inputs. These row are separated into 5 parts: block A
    # (1st N random sets of inputs), block B (2nd N random sets of inputs), and 3 C
    # blocks (each C block is a combination of blocks A and B where one input from
    # block A is taken and the other two inputs are taken from block B). UQpy runs
    # SCIANTIX on every row. It then compares the outputs of block A with the outputs
    # of that input's C block. A and C share that only one input so if their outputs move together,
    # that input has a large part of the output variance. The first order index is covariance between A and C
    # divided by the total variance for each input.
    N, d = N_SAMPLES, len(input_names)
    X_all = np.array(sm.INPUTS)
    print(f"{len(sm.OUTPUTS)} runs = N x (d+2) = {N} x {d+2}: block A, block B, then one block C per input")
    A, B = X_all[:N], X_all[N:2*N]
    print("first row of A (1st random set of inputs):", A[0])
    print("first row of B (2nd random set of inputs):", B[0])
    print("C Blocks (Take one input from A block and the other two inputs from B block)")
    for i, n in enumerate(input_names):
        C_i = X_all[2*N + i*N : 2*N + (i+1)*N]
        from_A = [input_names[k] for k in range(d) if np.allclose(C_i[:, k], A[:, k])]
        from_B = [input_names[k] for k in range(d) if np.allclose(C_i[:, k], B[:, k])]
        print(f"C for {n:<13}:", C_i[0], " copied from A:", from_A, " from B:", from_B)
    print(f"{len(sm.OUTPUTS)} SCIANTIX runs took {time.time() - t0:.0f} s")

    # =================== Results ===================
    S1 = np.asarray(sobol.first_order_indices)      # (n_inputs, n_outputs)
    ST = np.asarray(sobol.total_order_indices)
    Y = np.array(sm.OUTPUTS)                        # (n_runs, n_outputs): raw outputs of every run

    # TODO: Write out TSI results
    rows = []
    for j in sorted(names):
        col = Y[:, j]
        if not np.isfinite(col).all():
            status = "has NaN/inf"
        elif np.ptp(col) <= 1e-9 * max(np.abs(col).max(), 1e-300):
            status = "constant"         # does not change with the inputs: no index exists
        else:
            status = "ok"

        row = {"index": j, "output": names[j], "status": status}
        for k, n in enumerate(input_names):
            row[f"S1_{n}"] = S1[k, j] if status == "ok" else np.nan
        rows.append(row)

    df = pd.DataFrame(rows)
    s1_cols = [f"S1_{n}" for n in input_names]
    df["sum_S1"] = df[s1_cols].sum(axis=1, min_count=1)     # near 1: additive; well below 1: interactions
    df["dominant_input"] = ""
    is_ok = df["status"] == "ok"
    df.loc[is_ok, "dominant_input"] = df.loc[is_ok, s1_cols].idxmax(axis=1).str.replace("S1_", "", regex=False)
    df.to_csv("sobol_indices.csv", index=False)

    pd.set_option("display.width", 220)
    pd.set_option("display.max_rows", 200)
    print(df["status"].value_counts().to_string(), "\n")
    print(df[is_ok][["output"] + s1_cols + ["sum_S1"]].round(2).to_string(index=False))
    print("\nOutputs dominated by each input (largest first-order index):")
    print(df[is_ok]["dominant_input"].value_counts().to_string())
    print("\nSaved sobol_indices.csv")

    colors = ['tab:blue', 'tab:orange', 'tab:green']
    for k, n in enumerate(input_names):
        plot_data = df[is_ok].set_index("output")[f"S1_{n}"].sort_values()

        fig, ax = plt.subplots(figsize=(20, 0.3 * len(plot_data) + 1))   # taller for more outputs
        ax.barh(plot_data.index, plot_data.values, color=colors[k])
        ax.set_xlabel(f"First Order Sobol Indices for {n.capitalize()}")
        plt.tight_layout()
        plt.savefig(f"sobol_{n}.png", dpi=200)
        plt.close(fig)
