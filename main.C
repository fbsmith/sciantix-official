#include <stdio.h>
#include <TUSrcCoupling.h>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

int* getSciantixOptions() {
    int* Sciantix_options = new int[23];
    Sciantix_options[ 0] = 1;       // iGrainGrowth (0= no grain growth, 1= Ainscough et al. (1973), 2= Van Uffelen et al. (2013))
    Sciantix_options[ 1] = 1;       // iFissionGasDiffusivity (0= constant value, 1= Turnbull et al. (1988))
    Sciantix_options[ 2] = 1;       // iDiffusionSolver (1= SDA with quasi-stationary hypothesis, 2= SDA without quasi-stationary hypothesis)
    Sciantix_options[ 3] = 1;       // iIntraGranularBubbleBehavior (1= Pizzocri et al. (2018))
    Sciantix_options[ 4] = 1;       // iResolutionRate (0= constant value, 1= Turnbull (1971), 2= Losonen (2000), 3= thermal resolution, Cognini et al. (2021))
    Sciantix_options[ 5] = 1;       // iTrappingRate (0= constant value, 1= Ham (1958))
    Sciantix_options[ 6] = 1;       // iNucleationRate (0= constant value, 1= Olander, Wongsawaeng (2006))
    Sciantix_options[ 7] = 1;       // iOutput (1= default output files)
    Sciantix_options[ 8] = 1;       // iGrainBoundaryVacancyDiffusivity (0= constant value, 1= Reynolds and Burton (1979), 2= White (2004))
    Sciantix_options[ 9] = 1;       // iGrainBoundaryBehaviour (0= no grain boundary bubbles, 1= Pastore et al (2013))
    Sciantix_options[10] = 1;       // iGrainBoundaryMicroCracking (0= no model considered, 1= Barani et al. (2017))
    Sciantix_options[11] = 0;       // iFuelMatrix (0= UO2, 1= UO2 + HBS)
    Sciantix_options[12] = 0;       // iGrainBoundaryVenting (0= no model considered, 1= Pizzocri et al., D6.4 (2020), H2020 Project INSPYRE)
    Sciantix_options[13] = 0;       // iRadioactiveFissionGas (0= not considered)
    Sciantix_options[14] = 0;       // iHelium (0= not considered)
    Sciantix_options[15] = 0;       // iHeDiffusivity (0= null value, 1= limited lattice damage, Luzzi et al. (2018), 2= significant lattice damage, Luzzi et al. (2018))
    Sciantix_options[16] = 0;       // iGrainBoundarySweeping (0= no model considered, 1= TRANSURANUS swept volume model)
    Sciantix_options[17] = 0;       // iHighBurnupStructureFormation (0= no model considered, 1= fraction of HBS-restructured volume from Barani et al. (2020))
    Sciantix_options[18] = 0;       // iHighBurnupStructurePorosity (0= no evolution of HBS porosity, 1= HBS porosity evolution based on Spino et al. (2006) data)
    Sciantix_options[19] = 0;       // iHeliumProductionRate (0= zero production rate, 1= helium from ternary fissions, 2= linear with burnup (FR))
    Sciantix_options[20] = 0;       // iStoichiometryDeviation (0= not considered, 1= Cox et al. 1986, 2= Bittel et al. 1969, 3= Abrefah et al. 1994, 4= Imamura et al. 1997, 5= Langmuir-based approach)
    Sciantix_options[21] = 0;       // iBubbleDiffusivity (0= not considered, 1= volume diffusivity)
    Sciantix_options[22] = 0;       // iChromiumSolubility (0= not considered, 1= chromium solubility in UO2 based on Barani et al. (2020))

    return Sciantix_options;
}

double* getSciantixHistory() {
    double* Sciantix_history = new double[8];

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
    double* Sciantix_variables = new double[161];   

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
    Sciantix_variables[ 25] = 20.0;     // Intergranular bubble concentration, bub/m2
                                        // Value taken from White (2004) and converted from 20/um^2 to /m2
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

std::string* getSciantixVariablesStrings() {
    std::string headers = "";
    std::string values  = "";
    double* Sciantix_variables = getSciantixVariables();
    
    /******************** Grain Radius ********************/
    headers += "Grain radius (m),";
    values += std::to_string(Sciantix_variables[0]) +  ",";
    
    /******************** Xe Variables ********************/
    headers += "Xe produced (at/m3),";
    values += std::to_string(Sciantix_variables[1]) +  ",";
    headers += "Xe produced in HBS (at/m3),";
    values += std::to_string(Sciantix_variables[100]) +  ",";
    headers += "Xe in grain (at/m3),";
    values += std::to_string(Sciantix_variables[2]) +  ",";
    headers += "Xe in grain HBS (at/m3),";
    values += std::to_string(Sciantix_variables[92]) +  ",";
    headers += "Xe in intragranular solution (at/m3),";
    values += std::to_string(Sciantix_variables[3]) +  ",";
    headers += "Xe in intragranular bubbles (at/m3),";
    values += std::to_string(Sciantix_variables[4]) +  ",";
    headers += "Xe at grain boundary (at/m3),";
    values += std::to_string(Sciantix_variables[5]) +  ",";
    headers += "Xe released (at/m3),";
    values += std::to_string(Sciantix_variables[6]) +  ",";
    headers += "Xe decayed initialized at 0.0,";
    values += std::to_string(0.0) + ",";
    
    /******************** Kr Variables ********************/
    headers += "Kr produced (at/m3),";
    values += std::to_string(Sciantix_variables[7]) +  ",";
    headers += "Kr in grain (at/m3),";
    values += std::to_string(Sciantix_variables[8]) +  ",";
    headers += "Kr in intragranular solution (at/m3),";
    values += std::to_string(Sciantix_variables[9]) +  ",";
    headers += "Kr in intragranular bubbles (at/m3),";
    values += std::to_string(Sciantix_variables[10]) +  ",";
    headers += "Kr at grain boundary (at/m3),";
    values += std::to_string(Sciantix_variables[11]) +  ",";
    headers += "Kr released (at/m3),";
    values += std::to_string(Sciantix_variables[12]) +  ",";
    headers += "Kr decayed initialized at 0.0,";
    values += std::to_string(0.0) + ",";

    /******************** He Variables ********************/
    headers += "He produced (at/m3),";
    values += std::to_string(Sciantix_variables[13]) +  ",";
    headers += "He in grain (at/m3),";
    values += std::to_string(Sciantix_variables[14]) +  ",";
    headers += "He in intragranular solution (at/m3),";
    values += std::to_string(Sciantix_variables[15]) +  ",";
    headers += "He in intragranular bubbles (at/m3),";
    values += std::to_string(Sciantix_variables[16]) +  ",";
    headers += "He at grain boundary (at/m3),";
    values += std::to_string(Sciantix_variables[17]) +  ",";
    headers += "He at grain boundary (alias/variant) (at/m3),";
    values += std::to_string(Sciantix_variables[71]) +  ",";
    headers += "He released (at/m3),";
    values += std::to_string(Sciantix_variables[18]) +  ",";
    headers += "He decayed initialized at 0.0,";
    values += std::to_string(0.0) + ",";

    /******************** Xe133 Variables ********************/
    headers += "Xe133 produced (at/m3),";
    values += std::to_string(Sciantix_variables[48]) +  ",";
    headers += "Xe133 in grain (at/m3),";
    values += std::to_string(Sciantix_variables[49]) +  ",";
    headers += "Xe133 in intragranular solution (at/m3),";
    values += std::to_string(Sciantix_variables[50]) +  ",";
    headers += "Xe133 in intragranular bubbles (at/m3),";
    values += std::to_string(Sciantix_variables[51]) +  ",";
    headers += "Xe133 decayed (at/m3),";
    values += std::to_string(Sciantix_variables[52]) +  ",";
    headers += "Xe133 at grain boundary (at/m3),";
    values += std::to_string(Sciantix_variables[53]) +  ",";
    headers += "Xe133 released (at/m3),";
    values += std::to_string(Sciantix_variables[54]) +  ",";

    /******************** Kr85m Variables ********************/
    headers += "Kr85m produced (at/m3),";
    values += std::to_string(Sciantix_variables[57]) +  ",";    
    headers += "Kr85m in grain (at/m3),";
    values += std::to_string(Sciantix_variables[58]) +  ",";
    headers += "Kr85m in intragranular solution (at/m3),";
    values += std::to_string(Sciantix_variables[59]) +  ",";
    headers += "Kr85m in intragranular bubbles (at/m3),";
    values += std::to_string(Sciantix_variables[60]) +  ",";
    headers += "Kr85m decayed (at/m3),";
    values += std::to_string(Sciantix_variables[61]) +  ",";
    headers += "Kr85m at grain boundary (at/m3),";
    values += std::to_string(Sciantix_variables[62]) +  ",";
    headers += "Kr85m released (at/m3),";
    values += std::to_string(Sciantix_variables[63]) +  ",";

    /******************** Intragranular Bubbles ********************/
    headers += "Intragranular bubble concentration (bub/m3),";
    values += std::to_string(Sciantix_variables[19]) +  ",";
    headers += "Intragranular bubble radius (m),";
    values += std::to_string(Sciantix_variables[20]) +  ",";
    headers += "Xe atoms per intragranular bubble,";
    values += std::to_string(Sciantix_variables[21]) +  ",";
    headers += "Kr atoms per intragranular bubble,";
    values += std::to_string(Sciantix_variables[22]) +  ",";
    headers += "He atoms per intragranular bubble,";
    values += std::to_string(Sciantix_variables[23]) +  ",";
    headers += "Intragranular gas bubble swelling (%),";
    values += std::to_string(Sciantix_variables[24]) +  ",";
    headers += "Intragranular gas solution swelling (%),";
    values += std::to_string(Sciantix_variables[68]) +  ",";

    /******************** Intergranular Bubbles ********************/
    headers += "Intergranular bubble concentration (bub/m2),";
    values += std::to_string(Sciantix_variables[25]) +  ",";
    headers += "Xe atoms per intergranular bubble,";
    values += std::to_string(Sciantix_variables[26]) +  ",";    
    headers += "Kr atoms per intergranular bubble,";
    values += std::to_string(Sciantix_variables[27]) +  ",";
    headers += "He atoms per intergranular bubble,";
    values += std::to_string(Sciantix_variables[28]) +  ",";
    headers += "Intergranular atoms per bubble,";
    values += std::to_string(Sciantix_variables[29]) +  ",";
    headers += "Intergranular vacancies per bubble,";
    values += std::to_string(Sciantix_variables[30]) +  ",";
    headers += "Intergranular bubble radius (m),";
    values += std::to_string(Sciantix_variables[31]) +  ",";
    headers += "Intergranular bubble area (m2),";
    values += std::to_string(Sciantix_variables[32]) +  ",";
    headers += "Intergranular bubble volume (m3),";
    values += std::to_string(Sciantix_variables[33]) +  ",";
    headers += "Intergranular fractional coverage (%),";
    values += std::to_string(Sciantix_variables[34]) +  ",";
    headers += "Intergranular saturation fractional coverage (%),";
    values += std::to_string(Sciantix_variables[35]) +  ",";
    headers += "Intergranular gas swelling (%),";
    values += std::to_string(Sciantix_variables[36]) +  ",";
    headers += "Intergranular fractional intactness (%),";
    values += std::to_string(Sciantix_variables[37]) +  ",";

    /******************** Burnup and Material Properties ********************/
    headers += "Burnup (MWd/kgUO2),";
    values += std::to_string(Sciantix_variables[38]) +  ",";
    headers += "FIMA (%),";
    values += std::to_string(Sciantix_variables[69]) +  ",";
    headers += "Effective burnup (MWd/kgUO2),";
    values += std::to_string(Sciantix_variables[39]) +  ",";
    headers += "Irradiation time (h),";
    values += std::to_string(Sciantix_variables[65]) +  ",";
    headers += "Fuel density (kg/m3),";
    values += std::to_string(Sciantix_variables[40]) +  ",";
    headers += "U234 (at/m3),";
    values += std::to_string(Sciantix_variables[41]) +  ",";
    headers += "U235 (at/m3),";
    values += std::to_string(Sciantix_variables[42]) +  ",";
    headers += "U236 (at/m3),";
    values += std::to_string(Sciantix_variables[43]) +  ",";
    headers += "U237 (at/m3),";
    values += std::to_string(Sciantix_variables[44]) +  ",";
    headers += "U238 (at/m3),";
    values += std::to_string(Sciantix_variables[45]) +  ",";

    /******************** Intergranular Venting ********************/
    headers += "Intergranular vented fraction (%),";
    values += std::to_string(Sciantix_variables[46]) +  ",";
    headers += "Intergranular venting probability (%),";
    values += std::to_string(Sciantix_variables[47]) +  ",";
    
    /******************** HBS Structure ********************/
    headers += "Restructured volume fraction (%),";
    values += std::to_string(Sciantix_variables[55]) +  ",";
    headers += "Intragranular similarity ratio (%),";
    values += std::to_string(Sciantix_variables[64]) +  ",";
    headers += "Stoichiometry deviation (%),";
    values += std::to_string(Sciantix_variables[66]) +  ",";
    headers += "Fuel oxygen partial pressure (MPa),";
    values += std::to_string(Sciantix_variables[67]) +  ",";
    headers += "HBS porosity (%),";
    values += std::to_string(Sciantix_variables[56]) +  ",";
    headers += "HBS pore density (pores/m3),";
    values += std::to_string(Sciantix_variables[80]) +  ",";
    headers += "HBS pore volume (m3),";
    values += std::to_string(Sciantix_variables[81]) +  ",";
    headers += "HBS pore radius (m),";
    values += std::to_string(Sciantix_variables[82]) +  ",";
    headers += "Xe in HBS pores (at/m3),";
    values += std::to_string(Sciantix_variables[83]) +  ",";
    headers += "Xe in HBS pores - variance (at^2/m3),";
    values += std::to_string(Sciantix_variables[85]) +  ",";
    headers += "Xe atoms per HBS pore (at/pore),";
    values += std::to_string(Sciantix_variables[86]) +  ",";
    headers += "Xe atoms per HBS pore - variance (at^2/pore),";
    values += std::to_string(Sciantix_variables[88]) +  ",";

    /******************** Chromium & Lattice ********************/
    headers += "Chromium content (µg/g),";
    values += std::to_string(Sciantix_variables[150]) +  ",";
    headers += "Lattice parameter (m),";
    values += std::to_string(Sciantix_variables[151]) +  ",";
    headers += "Theoretical density (kg/m3),";
    values += std::to_string(Sciantix_variables[152]) +  ",";
    headers += "Chromium solubility (% weight/UO2),";
    values += std::to_string(Sciantix_variables[153]) +  ",";
    headers += "Chromia solubility (% weight/UO2),";
    values += std::to_string(Sciantix_variables[154]) +  ",";
    headers += "Chromium solution (at/m3),";
    values += std::to_string(Sciantix_variables[155]) +  ",";
    headers += "Chromium precipitate (at/m3),";
    values += std::to_string(Sciantix_variables[156]) +  ",";
    headers += "Chromia solution (at/m3),";
    values += std::to_string(Sciantix_variables[157]) +  ",";
    headers += "Chromia precipitate (at/m3),";
    values += std::to_string(Sciantix_variables[158]) +  ",";

    /******************** Diffusion ********************/
    headers += "Diffusion coefficient (m2/s),";
    values += std::to_string(Sciantix_variables[160]) + ",";

    delete[] Sciantix_variables;

    return new std::string[2]{headers, values};
}

double* getSciantixScalingFactors() {
    double* Sciantix_scaling_factors = new double[9];
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
    For SciantixDiffusionModes, this flattened array handles diffusion modes,
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
    return new double[680];
}

void inputToCSV(std::string filename) {
    std::cout << "Entering inputToCSV function" << std::endl;

    // header and value strings *Note to Aaron: Write everything as one string (Options headers, history headers, variable headers. Then Option variables, history variables, etc..)
    std::string headers = "";
    std::string values = "";

    // make file
    std::ofstream file(filename);

    // check if file opened successfully
    if (!file.is_open()) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return;
    }

    std::cout << "Writing to csv file" << std::endl;
    // write options to CSV;
    headers += "Filename,iGrainGrowth,iFissionGasDiffusivity,iDiffusionSolver,"
            "iIntraGranularBubbleBehavior,iResolutionRate,iTrappingRate,iNucleationRate,"
            "iOutput,iGrainBoundaryVacancyDiffusivity,iGrainBoundaryBehaviour,"
            "iGrainBoundaryMicroCracking,iFuelMatrix,iGrainBoundaryVenting,iRadioactiveFissionGas,"
            "iHelium,iHeDiffusivity,iGrainBoundarySweeping,iHighBurnupStructureFormation,"
            "iHighBurnupStructurePorosity,iHeliumProductionRate,iStoichiometryDeviation,"
            "iBubbledifusivity,iChromiumSolubility,";
    values += filename;
    values += ",";
    int* Sciantix_options = getSciantixOptions();
    for (int i = 0; i < 23; i++) {
        values += std::to_string(Sciantix_options[i]);
        values += ",";
    }
    delete[] Sciantix_options;

    // write history to CSV
    headers += "Time (hr),Temperature (K),Fission Rate (fissions/m^3/s),Hydrostatic Stress (MPa),"
            "Time (hr),Temperature (K),Fission Rate (fissions/m^3/s),Hydrostatic Stress (MPa),";
    double* Sciantix_history = getSciantixHistory();
    for (int i = 0; i < 8; i++) {
        values += std::to_string(Sciantix_history[i]);
        values += ",";
    }
    delete[] Sciantix_history;

    // write variables to CSV
    std::string* variable_string = getSciantixVariablesStrings();
    headers += variable_string[0];
    values  += variable_string[1];
    delete[] variable_string;

    // write scaling factors to CSV
    headers += "Resolution rate, Trapping rate, Nucleation rate, Diffusivity, Temperature, " 
               "Fission rate, Cent parameter, Helium production rate, Dummy,";
    double* Sciantix_scaling_factors = getSciantixScalingFactors();
    for (int i = 0; i < 9; i++) {
        values += std::to_string(Sciantix_scaling_factors[i]);
        values += ",";
    }
    delete[] Sciantix_scaling_factors;

    headers += "\n";
    values += "\n";
    file << headers;
    file << values;

    // close file
    file.close();
}

void outputToCSV(std::string filename) {
    // open CSV file

    // write variables to CSV

    // close file
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

    inputToCSV("filename1.csv");

    // call SCIANTIX
    printf("Calling SCIANTIX...\n\n");

    std::clock_t cpu_start = std::clock();
    
    callSciantix(
        Sciantix_options, 
        Sciantix_history, 
        Sciantix_variables, 
        Sciantix_scaling_factors, 
        Sciantix_diffusion_modes
    );

    std::clock_t cpu_end = std::clock();

    printf("SCIANTIX called successfully.\n\n");

    // print output from SCIANTIX
    printf("Output from SCIANTIX...\n\n");

    double cpu_time_seconds = static_cast<double>(cpu_end - cpu_start) / CLOCKS_PER_SEC;
    printf("SCIANTIX clock time: %.3e s\n\n", cpu_time_seconds);

    printf("SCIANTIX output processed successfully.\n\n");

    delete[] Sciantix_options;
    delete[] Sciantix_history;
    delete[] Sciantix_variables;
    delete[] Sciantix_scaling_factors;
    delete[] Sciantix_diffusion_modes;
    
    return 0;
}
