#include <stdio.h>
#include <Sciantix.h>

int* getSciantixOptions() {
    int* Sciantix_options = (int*)malloc(23 * sizeof(int));
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
    Sciantix_options[23] = 0;    // iChromiumSolubility (0= not considered, 1= chromium solubility in UO2 based on Barani et al. (2020))

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
    double* Sciantix_variables = (double*)calloc(161, sizeof(double));   

    Sciantix_variables[  0] = 5.0e-06;  // Grain radius, m

    // Xe
    Sciantix_variables[  1] = 0.0;      // Xe produced, at/m3
    Sciantix_variables[100] = 0.0;      // Xe produced in HBS, at/m3
    Sciantix_variables[  2] = 0.0;      // Xe in grain, at/m3
    Sciantix_variables[ 92] = 0.0;      // Xe in grain HBS, at/m3
    Sciantix_variables[  3] = 0.0;      // Xe in intragranular solution, at/m3
    Sciantix_variables[  4] = 0.0;      // Xe in intragranular bubbles, at/m3
    Sciantix_variables[  5] = 0.0;      // Xe at grain boundary, at/m3
    Sciantix_variables[  6] = 0.0;      // Xe released, at/m3
                                        // Xe decayed initialized at 0.0

    // Kr
    Sciantix_variables[  7] = 0.0;      // Kr produced, at/m3
    Sciantix_variables[  8] = 0.0;      // Kr in grain, at/m3
    Sciantix_variables[  9] = 0.0;      // Kr in intragranular solution, at/m3
    Sciantix_variables[ 10] = 0.0;      // Kr in intragranular bubbles, at/m3
    Sciantix_variables[ 11] = 0.0;      // Kr at grain boundary, at/m3
    Sciantix_variables[ 12] = 0.0;      // Kr released, at/m3
                                        // Kr decayed initialized at 0.0

    // He
    Sciantix_variables[ 13] = 0.0;      // He produced, at/m3
    Sciantix_variables[ 14] = 0.0;      // He in grain, at/m3
    Sciantix_variables[ 15] = 0.0;      // He in intragranular solution, at/m3
    Sciantix_variables[ 16] = 0.0;      // He in intragranular bubbles, at/m3
    Sciantix_variables[ 17] = 0.0;      // He at grain boundary, at/m3
    Sciantix_variables[ 71] = 0.0;      // He at grain boundary (alias/variant), at/m3
    Sciantix_variables[ 18] = 0.0;      // He released, at/m3
                                        // He decayed initialized at 0.0

    // Radioactive FG - Xe133
    Sciantix_variables[ 48] = 0.0;      // Xe133 produced, at/m3
    Sciantix_variables[ 49] = 0.0;      // Xe133 in grain, at/m3
    Sciantix_variables[ 50] = 0.0;      // Xe133 in intragranular solution, at/m3
    Sciantix_variables[ 51] = 0.0;      // Xe133 in intragranular bubbles, at/m3
    Sciantix_variables[ 52] = 0.0;      // Xe133 decayed, at/m3
    Sciantix_variables[ 53] = 0.0;      // Xe133 at grain boundary, at/m3
    Sciantix_variables[ 54] = 0.0;      // Xe133 released, at/m3

    // Radioactive FG - Kr85m
    Sciantix_variables[ 57] = 0.0;      // Kr85m produced, at/m3
    Sciantix_variables[ 58] = 0.0;      // Kr85m in grain, at/m3
    Sciantix_variables[ 59] = 0.0;      // Kr85m in intragranular solution, at/m3
    Sciantix_variables[ 60] = 0.0;      // Kr85m in intragranular bubbles, at/m3
    Sciantix_variables[ 61] = 0.0;      // Kr85m decayed, at/m3
    Sciantix_variables[ 62] = 0.0;      // Kr85m at grain boundary, at/m3
    Sciantix_variables[ 63] = 0.0;      // Kr85m released, at/m3

    // Intragranular bubbles
    Sciantix_variables[ 19] = 0.0;      // Intragranular bubble concentration, bub/m3
    Sciantix_variables[ 20] = 0.0;      // Intragranular bubble radius, m
    Sciantix_variables[ 21] = 0.0;      // Xe atoms per intragranular bubble
    Sciantix_variables[ 22] = 0.0;      // Kr atoms per intragranular bubble
    Sciantix_variables[ 23] = 0.0;      // He atoms per intragranular bubble
    Sciantix_variables[ 24] = 0.0;      // Intragranular gas bubble swelling, /
    Sciantix_variables[ 68] = 0.0;      // Intragranular gas solution swelling, /

    // Intergranular bubbles
    Sciantix_variables[ 25] = 0.0;      // Intergranular bubble concentration, bub/m2
    Sciantix_variables[ 26] = 0.0;      // Xe atoms per intergranular bubble
    Sciantix_variables[ 27] = 0.0;      // Kr atoms per intergranular bubble
    Sciantix_variables[ 28] = 0.0;      // He atoms per intergranular bubble
    Sciantix_variables[ 29] = 0.0;      // Intergranular atoms per bubble
    Sciantix_variables[ 30] = 0.0;      // Intergranular vacancies per bubble
    Sciantix_variables[ 31] = 0.0;      // Intergranular bubble radius, m
    Sciantix_variables[ 32] = 0.0;      // Intergranular bubble area, m2
    Sciantix_variables[ 33] = 0.0;      // Intergranular bubble volume, m3
    Sciantix_variables[ 34] = 0.0;      // Intergranular fractional coverage, /
    Sciantix_variables[ 35] = 0.0;      // Intergranular saturation fractional coverage, /
    Sciantix_variables[ 36] = 0.0;      // Intergranular gas swelling, /
    Sciantix_variables[ 37] = 0.0;      // Intergranular fractional intactness, /

    // Burnup and material properties
    Sciantix_variables[ 38] = 0.0;      // Burnup, MWd/kgUO2
    Sciantix_variables[ 69] = 0.0;      // FIMA, %
    Sciantix_variables[ 39] = 0.0;      // Effective burnup, MWd/kgUO2
    Sciantix_variables[ 65] = 0.0;      // Irradiation time, h
    Sciantix_variables[ 40] = 10641.0;  // Fuel density, kg/m3
    Sciantix_variables[ 41] = 0.0;      // U234, at/m3
    Sciantix_variables[ 42] = 3.0;      // U235, at/m3
    Sciantix_variables[ 43] = 0.0;      // U236, at/m3
    Sciantix_variables[ 44] = 0.0;      // U237, at/m3
    Sciantix_variables[ 45] = 97.0;     // U238, at/m3

    // Intergranular venting
    Sciantix_variables[ 46] = 0.0;      // Intergranular vented fraction, /
    Sciantix_variables[ 47] = 0.0;      // Intergranular venting probability, /

    // HBS structure
    Sciantix_variables[ 55] = 0.0;      // Restructured volume fraction, /
    Sciantix_variables[ 64] = 0.0;      // Intragranular similarity ratio, /
    Sciantix_variables[ 66] = 0.0;      // Stoichiometry deviation, /
    Sciantix_variables[ 67] = 0.0;      // Fuel oxygen partial pressure, MPa
    Sciantix_variables[ 56] = 0.0;      // HBS porosity, /
    Sciantix_variables[ 80] = 0.0;      // HBS pore density, pores/m3
    Sciantix_variables[ 81] = 0.0;      // HBS pore volume, m3
    Sciantix_variables[ 82] = 0.0;      // HBS pore radius, m
    Sciantix_variables[ 83] = 0.0;      // Xe in HBS pores, at/m3
    Sciantix_variables[ 85] = 0.0;      // Xe in HBS pores - variance, at^2/m3
    Sciantix_variables[ 86] = 0.0;      // Xe atoms per HBS pore, at/pore
    Sciantix_variables[ 88] = 0.0;      // Xe atoms per HBS pore - variance, at^2/pore

    // Chromium & lattice
    Sciantix_variables[150] = 0.0;      // Chromium content, µg/g
    Sciantix_variables[151] = 0.0;      // Lattice parameter, m
    Sciantix_variables[152] = 0.0;      // Theoretical density, kg/m3
    Sciantix_variables[153] = 0.0;      // Chromium solubility, % weight/UO2
    Sciantix_variables[154] = 0.0;      // Chromia solubility, % weight/UO2
    Sciantix_variables[155] = 0.0;      // Chromium solution, at/m3
    Sciantix_variables[156] = 0.0;      // Chromium precipitate, at/m3
    Sciantix_variables[157] = 0.0;      // Chromia solution, at/m3
    Sciantix_variables[158] = 0.0;      // Chromia precipitate, at/m3

    // Diffusion
    Sciantix_variables[160] = 0.0;      // Diffusion coefficient, m2/s

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

double* getSciantixDiffusionModes() {
    /*
    For SciantixDiffusionModes, this flattned array handles diffusion modes,
    diffusion mode solutions, and diffusion modes related to bubbles for
    each gas. Diffusion modes, diffusion mode solutions, and diffusion modes
    bubbles are 40 elements long, each, meaning each element requires 120
    elements in the flattened Sciantix_diffusion_modes array

    Xe
      0- 39 diffusion modes
     40- 79 diffusion modes related to solutions
     80-119 diffusion modes related to bubbles

    Kr
    120-159 diffusion modes
    160-199 diffusion modes related to solutions
    200-239 diffusion modes related to bubbles

    He
    240-279 diffusion modes
    280-319 diffusion modes related to solutions
    320-359 diffusion modes related to bubbles

    Xe133
    360-399 diffusion modes
    400-439 diffusion modes related to solutions
    440-479 diffusion modes related to bubbles

    Kr85m
    480-519 diffusion modes
    520-559 diffusion modes related to solutions
    560-599 diffusion modes related to bubbles

    Xe in HBS
    600-639 diffusion modes
    640-679 diffusion modes related to solutions
    */
    return (double*)calloc(680, sizeof(double));
}

int main() {
    // setup input to feed to SCIANTIX
    printf("Setting up input for SCIANTIX...\n\n");

    // get Sciantix inputs
    int* Sciantix_options = getSciantixOptions();
    double* Sciantix_history = getSciantixHistory();
    double* Sciantix_variables = getSciantixVariables();    
    double* Sciantix_scaling_factors = getSciantixScalingFactors();
    double* Sciantix_diffusion_modes = getSciantixDiffusionModes();

    printf("Finished setting up input.\n\n");

    // call SCIANTIX
    printf("Calling SCIANTIX...\n\n");
    Sciantix(
        Sciantix_options, 
        Sciantix_history, 
        Sciantix_variables, 
        Sciantix_scaling_factors, 
        Sciantix_diffusion_modes
    );
    printf("SCIANTIX called successfully.\n\n");

    // print output from SCIANTIX
    printf("Output from SCIANTIX...\n\n");

    for (int i = 0; i < 161; i++) {
        printf("Sciantix_variables[%d] = %e\n", i, Sciantix_variables[i]);
    }
    printf("\n");

    printf("SCIANTIX output processed successfully.\n\n");

    free(Sciantix_options);
    free(Sciantix_history);
    free(Sciantix_variables);
    free(Sciantix_scaling_factors);
    free(Sciantix_diffusion_modes);
    
    return 0;
}
