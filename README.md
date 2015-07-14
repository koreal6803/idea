# Power-supply-noise-aware test pattern timing analysis and regeneration

## Indroduction

This framework contain several excution files
* sim_test : Run time-logic simulation
* idea_test : Run idea simulation and verify the results
* pat_regene : Run pattern regeneration to maintain fault coverage
* find_ppo : simulate ppi2 for test patterns
* path_gene : generate spice file (with/without power grid) and perfrom HSPICE/NANOSIM simulation automatically

## Input files

For basic time-logic-simulation, we need those files to perform simulation
* circuit.v : the circuit compiled by CADENCE encounder
* standard_library.lib : contain timing informations and internal current informations
* pattern.pat : the pattern file is connverted from stil using fixStil.out and stil2pat.py

For idea simulation, we need all files above and other additional files:
* circuit.dspf : the power grid RC information of the circuit and top module pin capacitance
* circuit.spf : netlist RC informations of circuit

For pattern regeneration, we perfrom idea simulation to all patterns and find out risky patterns. So we need all files above and other files as well:
* idea.log : idea simulation result for each pattern
* wgl file : wgl file of the patterns set

## Input files setup
The benchmark file structure should be build as follow:
* benchmark_folder
  * circuit_name
    * circuit.v
    * circuit.pat
    * circuit.dspf
    * circuit.spf
    * circuit.wgl
    * circuit.sp
  * Library
    * library.lib
    * library.v
  * delay
    * v1.1.delay
    * v0.9.delay
    * ...

If you want to perform path generation to build SPICE files for each patterns, there are many important pre-setup:
* change workspace directory in pkg/idea/src/path_gene.cpp: line 294 to get the correct workspace. The workspace can be anywhere you want.
* make sure the CIRCUIT.SP have correct include files, otherwise path_gene.sh can not perform HSPICE simulation. 

## Usage

Perform timed-logic simulation
> ./script/sim_test **<CIRCUIT_NAME>**

Perform idea simulation under specific supply voltage
> ./script/idea_test.sh **<CIRCUIT_NAME> <VOLTAGE>**

Perform pattern regeneration
> ./script/pat_regene.sh **<LIB.V> <CIRCUIT.V> <CIRCUIT_NAME> <WGL> <IDEA_LOG><METHOD_ID> <ITERATION>**

Simulate ppi2 for test patterns
> ./script/find_ppo.sh **<CIRCUIT_NAME>**

Generate spice file (with/without power grid) and perform HSPICE/NANOSIM simulation automatically
> ./script/path_gene.sh **<CIRCUIT_NAME> <VOLTAGE> <NANOSIM/HSPICE>**

Perform idea simulation under specific pattern file
> ./script/idea_log.sh **<CIRCUIT_NAME> <VOLTAGE> <PAT>**

## Output File Format

For **sim_test.sh idea_test.sh, find_ppo.sh and idea_lo.sh**, the output only shows on terminal. (They will not generate any specific files)<br>
For **path_gene.sh**, there will be a new circuit directory in the workspace you setup, which contain every SPICE simulation results of each patterns.<br>
For **pat_regene.s**h, there will be a new circuit directory in the workspace you setup, it will show the regeneration result for method1 and method2 directory and each directory contain iteration directory to save logs of both ATPG and fault simulation.

## Package Usage

If you want to modify the c++ files, here is the package organization:

### Interface Package
Parse files to perform circuit simulations
* verilog
* standard delay format
* pattern file

### IR_Interface Package
Parse files related to power grid and library
* library
* dspf
* spf

### Core Package
Perform timed circuit simulation
* circuit builder
* circuit structure
* library parser
* library structure
* pattern set
* circuit simulator
* waveform

### IR Core Package
Perform power grid simulation
* RLC component
* power grid structure
* power grid voltage solver

### Matrix Package
KLU sparse matrix solver interface used for solving voltage
* matrix and vector structure
* solver

### IDEA Package
Perform pattern simulation
* verify to hspice 
* spice file generator
* idea simulation
* atpg regeneration

### Utility Package
* tetramax c++ interface
* HSPICE   c++ interface
* waveform viewer

## preconditions (Modify carefully!)
Here is some preconditions that I based on to write the codes.  Please makesure your new codes will not violated these preconditions, otherwise the framework may crash.  You need to check the code very carefully!

In path_generator.cpp:
*  pin Q ID of SDFF is 0
* pin QN ID of SDFF is 1
* pin SI ID of SDFF is 2
* pin D ID of SDFF is 0

In verify.cpp:
* assume all flip-flop name are capital

All wires and power grid nodes are translate by following rule:
(Especially when you want to find a specific net by name)

> 1. small case only
> 2. erase every '\\'
> 3. change every '/' to '_'

