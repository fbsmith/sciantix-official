import numpy as np
import UQpy
import os
import matplotlib.pyplot as plt

A = np.array([0.001, 89.9, 5.54, 42.10, 0.78, 1.26,
              0.04, 0.79, 74.51, 4.32, 82.51, 41.62])

def g_model(x, **kwargs):
    x = np.atleast_2d(x)                      # shape (n_samples, 12)
    g_i = (np.abs(4 * x - 2) + A) / (1 + A)   # each column uses its own a_i
    return np.prod(g_i, axis=1)               # one output per sample

if __name__ == "__main__":
    k = len(A)

    py_model = UQpy.PythonModel(
        model_script=os.path.basename(__file__),
        model_object_name='g_model',
        model_object=g_model,
        var_names=[f"x{i}" for i in range(1, k + 1)],
    )
    model = UQpy.RunModel(model=py_model)

    joint_dist = UQpy.JointIndependent([UQpy.Uniform(loc=0, scale=1) for _ in range(k)])

    morris = UQpy.MorrisSensitivity(
        runmodel_object=model,
        distributions=joint_dist,
        n_levels=4,
        maximize_dispersion=True,
    )
    morris.run(n_trajectories=10)   # 10 * (12 + 1) = 130 runs, same cost as Fig. 5

    mu_star = np.asarray(morris.mustar_indices)
    sigma = np.asarray(morris.sigma_indices)

    # Ranking check: smaller a_i should mean larger mu*
    print("Ranking by mu*:      ", (np.argsort(-mu_star) + 1).tolist())
    print("Expected (by a_i):   ", (np.argsort(A) + 1).tolist())

    # Plot like the paper's figure: label each point instead of a 12-entry legend
    fig, ax = plt.subplots()
    ax.scatter(mu_star, sigma, c="black", s=40)
    for i in range(k):
        ax.annotate(f"$X_{{{i+1}}}$", (mu_star[i], sigma[i]),
                    textcoords="offset points", xytext=(5, 3))
    ax.set_xlabel(r"$\mu^*$")
    ax.set_ylabel(r"$\sigma$")
    ax.set_title("g-function validation (k=12, N=130)")
    ax.grid(True, linestyle="--", alpha=0.5)
    fig.savefig("g_function_morris.png", dpi=150)
    print("Saved g_function_morris.png")
