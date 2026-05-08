# Future Extensions

## FPGA / Verilog Extension
Purpose:
Explore ultra-low-latency components commonly used in trading infrastructure.

Potential modules:
- low-latency market data parser simulator
- pre-trade risk gate
- timestamping path
- Verilog/SystemVerilog implementation
- Verilator-based simulation
- integration with C++ OpenLOB event path

Important:
This is not Phase 1. Matching correctness and simulation reproducibility come first.

## OCaml Extension
Purpose:
Explore type-safe strategy/scenario definitions inspired by functional programming workflows used in some quant firms.

Potential modules:
- strategy DSL
- simulation scenario DSL
- risk-rule definitions
- config generation for C++ simulator
- property-based testing ideas

Important:
OCaml is a future research extension, not a core dependency.
