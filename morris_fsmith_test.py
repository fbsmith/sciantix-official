import numpy as np
import UQpy
import os
import matplotlib.pyplot as plt

def fission_gas_model(x, **kwargs):
    """beta = Y_gas * F_dot  (fission gas production rate, atoms/m3/s)"""
    x = np.atleast_2d(x)
    Y_gas = x[:, 0]   # fission gas yield, atoms per fission
    F_dot = x[:, 1]   # fission rate, fissions/m3/s
    return Y_gas * F_dot

if __name__ == "__main__":
    # Input ranges: Uniform(loc, scale) covers [loc, loc + scale]
    Y_LO, Y_RANGE = 0.20, 0.10     # yield 0.20 - 0.30 (SCIANTIX uses ~0.27)
    F_LO, F_RANGE = 1e19, 2e19     # fission rate 1e19 - 3e19, same as your SCIANTIX study

    py_model = UQpy.PythonModel(
        model_script=os.path.basename(__file__),
        model_object_name='fission_gas_model',
        model_object=fission_gas_model,
        var_names=['Y_gas', 'F_dot'],
    )
    model = UQpy.RunModel(model=py_model)

    joint_dist = UQpy.JointIndependent([
        UQpy.Uniform(loc=Y_LO, scale=Y_RANGE),
        UQpy.Uniform(loc=F_LO, scale=F_RANGE),
    ])

    morris = UQpy.MorrisSensitivity(
        runmodel_object=model,
        distributions=joint_dist,
        n_levels=6,
        maximize_dispersion=True,
    )
    morris.run(n_trajectories=20)

    mu_star = np.asarray(morris.mustar_indices)
    sigma = np.asarray(morris.sigma_indices)

    predicted_mu = np.array([
        (F_LO + F_RANGE / 2) * Y_RANGE,   # Y_gas: EE = F * (range of Y)
        (Y_LO + Y_RANGE / 2) * F_RANGE,   # F_dot: EE = Y * (range of F)
    ])

    names = ['Y_gas', 'F_dot']
    print(f"{'factor':<8}{'mu* (Morris)':>15}{'mu* (predicted)':>18}{'sigma':>12}")
    for n, m, p, s in zip(names, mu_star, predicted_mu, sigma):
        print(f"{n:<8}{m:>15.3e}{p:>18.3e}{s:>12.3e}")

    fig, ax = plt.subplots()
    ax.scatter(mu_star, sigma, c="black", s=50)
    for i, label in enumerate([r"$Y_{gas}$", r"$\dot{F}$"]):
        ax.annotate(label, (mu_star[i], sigma[i]),
                    textcoords="offset points", xytext=(6, 4), fontsize=12)
    ax.set_xlabel(r"$\mu^*$")
    ax.set_ylabel(r"$\sigma$")
    ax.set_title(r"Fission gas source term, $\beta = Y_{gas}\dot{F}$")
    ax.grid(True, linestyle="--", alpha=0.5)
    fig.savefig("fission_gas_morris.png", dpi=150)
    print("Saved fission_gas_morris.png")