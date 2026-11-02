import numpy as np
import pandas as pd
import UQpy
import os

# Model Settings

SEED = 314
N_LEVELS = 6
N_TRAJ = 20

Y_FIXED = 0.27

# Input ranges: Uniform(loc, scale) covers [loc, loc + scale]
Y_LO, Y_RANGE = 0.20, 0.10     # yield 0.20 - 0.30 (SCIANTIX uses ~0.27)
F_LO, F_RANGE = 1e19, 2e19     # fission rate 1e19 - 3e19

names = ['Y_gas', 'F_dot']


def fission_gas_model(x, **kwargs):
    """beta = Y_gas * F_dot  (fission gas production rate, atoms/m3/s)"""
    Y_gas = Y_FIXED          # In output Y_gas is still perturbed, but outputted beta still uses Y_FIXED
    F_dot = x[:, 1]          # fission rate, fissions/m3/s
    beta = Y_gas * F_dot
    return beta


if __name__ == "__main__":

    # ---------------- Run Morris ----------------
    py_model = UQpy.PythonModel(
        model_script=os.path.basename(__file__),
        model_object_name='fission_gas_model',
        model_object=fission_gas_model,
        var_names=names,
    )
    model = UQpy.RunModel(model=py_model)

    joint_dist = UQpy.JointIndependent([
        UQpy.Uniform(loc=Y_LO, scale=Y_RANGE),
        UQpy.Uniform(loc=F_LO, scale=F_RANGE),
    ])

    morris = UQpy.MorrisSensitivity(
        runmodel_object=model,
        distributions=joint_dist,
        n_levels=N_LEVELS,
        maximize_dispersion=True,
        random_state=SEED,
    )
    morris.run(n_trajectories=N_TRAJ)

    U = np.asarray(morris.trajectories_unit_hypercube)    # (r, d+1, d) unit space
    X = np.asarray(morris.trajectories_physical_space)    # (r, d+1, d) physical units
    r, n_pts, d = X.shape
    delta = morris.delta
    ee_uqpy = np.asarray(morris.elementary_effects, dtype=float).reshape(r, d)
    mu_star_uqpy = np.asarray(morris.mustar_indices, dtype=float).reshape(d)
    sigma_uqpy = np.asarray(morris.sigma_indices, dtype=float).reshape(d)

    # ---------------- Hand calculation ----------------
    point_rows = []
    ee_hand = np.zeros((r, d))
    ee_analytic = np.zeros((r, d))

    for t in range(r):
        beta = fission_gas_model(X[t])        # beta at every point of trajectory t

        # Building trajectory point table using dictionaries
        for p in range(n_pts):
            point_rows.append({
                "trajectory": t, "point": p,
                **{f"u_{n}": U[t, p, k] for k, n in enumerate(names)},
                **{n: X[t, p, k] for k, n in enumerate(names)},
                "beta": beta[p], "beta_calculated": Y_FIXED * X[t, p, 1],
            })

        # Walking through each trajectory and calculating elementary effects
        for s in range(d):                      
            du = U[t, s + 1] - U[t, s]           # change in each input (unit space)
            dx = X[t, s + 1] - X[t, s]           # change in each input (physical units)
            k = np.argmax(du)                    # the input that moved

            dbeta = beta[s + 1] - beta[s]
            ee_hand_step = dbeta / delta

            ee_hand[t, k] = ee_hand_step

            # Calculated ee_analytical using [dbeta/dY, dbeta/dF]

    # ---------------- Indices ----------------
    mu_star_hand = np.mean(np.abs(ee_hand), axis=0)
    sigma_hand = np.std(ee_hand, axis=0)

    predicted_mu = np.array([
        0.0,                     
        Y_FIXED * F_RANGE,       # F_dot: EE = Y * (range of F)
    ])

    indices = pd.DataFrame({
        "factor": names,
        "mu_star_uqpy": mu_star_uqpy,
        "mu_star_hand": mu_star_hand,
        "mu_star_predicted": predicted_mu,
        "mu_star_difference": ((mu_star_uqpy - predicted_mu) / predicted_mu) * 100,
        "sigma_uqpy": sigma_uqpy,
        "sigma_hand": sigma_hand,
    })

    points = pd.DataFrame(point_rows)

    # ---------------- Export files ----------------
    indices.to_csv("fission_gas_indices.csv", index=False)
    points.to_csv("fission_gas_points.csv", index=False)
    print("\nSaved fission_gas_*.csv files")