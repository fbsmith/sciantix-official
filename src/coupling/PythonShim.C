#include "Initialization.h"
#include <vector>

extern "C" {

void init_sciantix(
    double Sciantix_history[],
    double Sciantix_variables[],
    double Sciantix_diffusion_modes[],
    double* Temperature_input, int temperature_len,
    double* Fissionrate_input, int fissionrate_len,
    double* Hydrostaticstress_input, int stress_len,
    double* Steampressure_input, int steam_len)
{
    std::vector<double> temperature(Temperature_input, Temperature_input + temperature_len);
    std::vector<double> fissionrate(Fissionrate_input, Fissionrate_input + fissionrate_len);
    std::vector<double> stress(Hydrostaticstress_input, Hydrostaticstress_input + stress_len);
    std::vector<double> steam(Steampressure_input, Steampressure_input + steam_len);

    Initialization(
        Sciantix_history,
        Sciantix_variables,
        Sciantix_diffusion_modes,
        temperature,
        fissionrate,
        stress,
        steam
    );
}

}