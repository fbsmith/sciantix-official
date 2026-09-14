import ctypes

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

temperature_input = (ctypes.c_double * 1)(1400.0)
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

xenon_released = []
xenon_produced = []
burnup = []
time_h = []

for i in range(50000):
    history[6] = 3600
    history[7] = i
    history[8] = i
    lib.callSciantix(options, history, variables, scaling_factors, diffusion_modes)
    xenon_released.append(variables[6])
    xenon_produced.append(variables[1])
    burnup.append(variables[38])
    time_h.append(variables[65])

    if i in (0, 1, 2, 10, 100, 1000, 5500, 49999):
        print(f"step {i}: Temperature={history[0]:.4}  Xe produced={variables[1]:.4e}  Xe released={variables[6]:.4e}  "
              f"Burnup={variables[38]:.4e}  IrrTime={variables[65]:.4e} Diffusion Coeff={variables[160]:.4e}")

