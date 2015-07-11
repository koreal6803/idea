#Power-supply-noise-aware test pattern timing analysis and regeneration


## Interface Package
Parse files to perform circuit simulations
* verilog
* standard delay format
* pattern file

## IR_Interface Package
Parse files related to power grid and library
* library
* dspf
* spf

## Core Package
Perform timed circuit simulation
* circuit builder
* circuit structure
* library parser
* library structure
* pattern set
* circuit simulator
* waveform

## IR Core Package
Perform power grid simulation
* RLC component
* power grid structure
* power grid voltage solver

## Matrix Package
KLU sparse matrix solver interface used for solving voltage
* matrix and vector structure
* solver

## IDEA Package
Perform pattern simulation
* verify to hspice 
* spice file generator
* idea simulation
* atpg regeneration

## Utility Package
* tetramax using c++ interface
* HSPICE   using c++ interface
* waveform viewer

## preconditions (modify carefully!)

* pin Q ID of SDFF is 0
* pin QN ID of SDFF is 1
* pin SI ID of SDFF is 2
* pin D ID of SDFF is 0

* in verify.cpp	: assume all flip-flop name are capital

* all wires and power grid nodes are translate by following rule:

> 1. small case only
2. erase every '\\'
3. change every '/' to '_'

