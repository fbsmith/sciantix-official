#include <stdio.h>
#include <Sciantix.h>

int* getSciantixOptions() {
    int* Sciantix_options = (int*)malloc(22 * sizeof(int));
    Sciantix_options[0] = 1;    // iGrainGrowth (0= no grain growth, 1= Ainscough et al. (1973), 2= Van Uffelen et al. (2013))
    Sciantix_options[1] = 1;    // iFissionGasDiffusivity (0= constant value, 1= Turnbull et al. (1988))
    Sciantix_options[2] = 1;    // iDiffusionSolver (1= SDA with quasi-stationary hypothesis, 2= SDA without quasi-stationary hypothesis)
    Sciantix_options[3] = 1;    // iIntraGranularBubbleBehavior (1= Pizzocri et al. (2018))
    Sciantix_options[4] = 1;    // iResolutionRate (0= constant value, 1= Turnbull (1971), 2= Losonen (2000), 3= thermal resolution, Cognini et al. (2021))
    Sciantix_options[5] = 1;    // iTrappingRate (0= constant value, 1= Ham (1958))
    Sciantix_options[6] = 1;    // iNucleationRate (0= constant value, 1= Olander, Wongsawaeng (2006))
    Sciantix_options[7] = 1;    // iOutput (1= default output files)
    Sciantix_options[8] = 1;    // iGrainBoundaryVacancyDiffusivity (0= constant value, 1= Reynolds and Burton (1979), 2= White (2004))
    Sciantix_options[9] = 1;    // iGrainBoundaryBehaviour (0= no grain boundary bubbles, 1= Pastore et al (2013))
    Sciantix_options[10] = 1;    // iGrainBoundaryMicroCracking (0= no model considered, 1= Barani et al. (2017))
    Sciantix_options[11] = 0;    // iFuelMatrix (0= UO2, 1= UO2 + HBS)
    Sciantix_options[12] = 0;    // iGrainBoundaryVenting (0= no model considered, 1= Pizzocri et al., D6.4 (2020), H2020 Project INSPYRE)
    Sciantix_options[13] = 0;    // iRadioactiveFissionGas (0= not considered)
    Sciantix_options[14] = 0;    // iHelium (0= not considered)
    Sciantix_options[15] = 0;    // iHeDiffusivity (0= null value, 1= limited lattice damage, Luzzi et al. (2018), 2= significant lattice damage, Luzzi et al. (2018))
    Sciantix_options[16] = 0;    // iGrainBoundarySweeping (0= no model considered, 1= TRANSURANUS swept volume model)
    Sciantix_options[17] = 0;    // iHighBurnupStructureFormation (0= no model considered, 1= fraction of HBS-restructured volume from Barani et al. (2020))
    Sciantix_options[18] = 0;    // iHighBurnupStructurePorosity (0= no evolution of HBS porosity, 1= HBS porosity evolution based on Spino et al. (2006) data)
    Sciantix_options[19] = 0;    // iHeliumProductionRate (0= zero production rate, 1= helium from ternary fissions, 2= linear with burnup (FR))
    Sciantix_options[20] = 0;    // iStoichiometryDeviation (0= not considered, 1= Cox et al. 1986, 2= Bittel et al. 1969, 3= Abrefah et al. 1994, 4= Imamura et al. 1997, 5= Langmuir-based approach)
    Sciantix_options[21] = 0;    // iBubbleDiffusivity (0= not considered, 1= volume diffusivity)

    return Sciantix_options;
}

double* getSciantixHistory() {
    double* Sciantix_history = (double*)malloc(8 * sizeof(double));

    Sciantix_history[0] = 0;      // time in hours
    Sciantix_history[1] = 1273;   // temperature in Kelvin
    Sciantix_history[2] = 1e19;   // fission rate in fissions/m^3/s
    Sciantix_history[3] = 0;      // hydrostatic stress in MPa

    Sciantix_history[4] = 5500;   // time in hours
    Sciantix_history[5] = 1273;   // temperature in Kelvin
    Sciantix_history[6] = 1e19;   // fission rate in fissions/m^3/s
    Sciantix_history[7] = 0;      // hydrostatic stress in MPa

    return Sciantix_history;
}

double* getSciantixVariables() {
    double* Sciantix_variables = (double*)malloc(41 * sizeof(double));

    // initial grain radius (m)
    Sciantix_variables[0] = 5.0e-06;

    // initial Xe (at/m³) produced, intragranular, intragranular in solution, intragranular in bubbles, grain boundary, released
    Sciantix_variables[1] = 0.0;
    Sciantix_variables[2] = 0.0;
    Sciantix_variables[3] = 0.0;
    Sciantix_variables[4] = 0.0;
    Sciantix_variables[5] = 0.0;
    Sciantix_variables[6] = 0.0;

    // initial Kr (at/m³) produced, intragranular, intragranular in solution, intragranular in bubbles, grain boundary, released
    Sciantix_variables[7] = 0.0;
    Sciantix_variables[8] = 0.0;
    Sciantix_variables[9] = 0.0;
    Sciantix_variables[10] = 0.0;
    Sciantix_variables[11] = 0.0;
    Sciantix_variables[12] = 0.0;

    // initial He (at/m³) produced, intragranular, intragranular in solution, intragranular in bubbles, grain boundary, released
    Sciantix_variables[13] = 0.0;
    Sciantix_variables[14] = 0.0;

    // initial intragranular bubble concentration (at/m³), radius (m)
    Sciantix_variables[15] = 0.0;
    Sciantix_variables[16] = 0.0;

    // initial fuel burn-up (MWd/kgUO2)
    Sciantix_variables[17] = 0.0;

    // initial fuel effective burn-up (MWd/kgUO2)
    Sciantix_variables[18] = 0.0;

    // initial irradiation time (h)
    Sciantix_variables[19] = 0.0;
    
    // initial fuel density (kg/m³)
    Sciantix_variables[20] = 10641.0;

    // initial U234 U235 U236 U237 U238 (% of heavy atoms) content
    Sciantix_variables[21] = 0.0;
    Sciantix_variables[22] = 3.0;
    Sciantix_variables[23] = 0.0;
    Sciantix_variables[24] = 0.0;
    Sciantix_variables[25] = 97.0;

    // initial Xe135 (at/m³) produced, intragranular, intragranular in solution, intragranular in bubbles, decayed, grain boundary, released
    Sciantix_variables[26] = 0.0;
    Sciantix_variables[27] = 0.0;
    Sciantix_variables[28] = 0.0;
    Sciantix_variables[29] = 0.0;
    Sciantix_variables[30] = 0.0;
    Sciantix_variables[31] = 0.0;
    Sciantix_variables[32] = 0.0;

    // initial Kr85 (at/m³) produced, intragranular, intragranular in solution, intragranular in bubbles, decayed, grain boundary, released
    Sciantix_variables[33] = 0.0;
    Sciantix_variables[34] = 0.0;
    Sciantix_variables[35] = 0.0;
    Sciantix_variables[36] = 0.0;
    Sciantix_variables[37] = 0.0;
    Sciantix_variables[38] = 0.0;
    Sciantix_variables[39] = 0.0;

    // iniitial fuel stoichiometry deviation (\)
    Sciantix_variables[40] = 0.0;

    return Sciantix_variables;
}

double* getSciantixScalingFactors() {
    double* Sciantix_scaling_factors = (double*)malloc(9 * sizeof(double));
    Sciantix_scaling_factors[0] = 1.0;   // scaling factor - resolution rate
    Sciantix_scaling_factors[1] = 1.0;   // scaling factor - trapping rate
    Sciantix_scaling_factors[2] = 1.0;   // scaling factor - nucleation rate
    Sciantix_scaling_factors[3] = 1.0;   // scaling factor - diffusivity
    Sciantix_scaling_factors[4] = 1.0;   // scaling factor - temperature
    Sciantix_scaling_factors[5] = 1.0;   // scaling factor - fission rate
    Sciantix_scaling_factors[6] = 1.0;   // scaling factor - cent parameter
    Sciantix_scaling_factors[7] = 1.0;   // scaling factor - helium production rate
    Sciantix_scaling_factors[8] = 1.0;   // scaling factor - dummy
    
    return Sciantix_scaling_factors;
}

int main() {
    // setup input to feed to SCIANTIX
    printf("Setting up input for SCIANTIX...\n");

    // get Sciantix inputs
    int* Sciantix_options = getSciantixOptions();
    double* Sciantix_history = getSciantixHistory();
    double* Sciantix_variables = getSciantixVariables();    
    double* Sciantix_scaling_factors = getSciantixScalingFactors();

    // Sciantix_diffusion_modes
    double Sciantix_diffusion_modes[720];

    printf("Finished setting up input.\n");

    // call SCIANTIX
    printf("Calling SCIANTIX...\n");
    Sciantix(
        Sciantix_options, 
        Sciantix_history, 
        Sciantix_variables, 
        Sciantix_scaling_factors, 
        Sciantix_diffusion_modes
    );
    printf("SCIANTIX called successfully.\n");

    // print output from SCIANTIX
    printf("Output from SCIANTIX...\n");
    printf("SCIANTIX output processed successfully.\n");

    free(Sciantix_options);
    free(Sciantix_history);
    free(Sciantix_variables);
    free(Sciantix_scaling_factors);
    
    return 0;
}
