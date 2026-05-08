# OpenLOB
### Open Market Microstructure Lab

OpenLOB is a C++ market microstructure research platform for simulating L3 limit order books, multi-agent order flow, queue position, latency, and AI-driven market-making strategies.

## What is OpenLOB?
OpenLOB is a deterministic, production-style C++20 research skeleton for market microstructure simulation. It is designed to evolve into a rigorous environment for studying market making, execution quality, and adverse selection under realistic order book dynamics.

## Why market makers care
Market makers need realistic simulation to understand spread capture, fill probability, queue dynamics, and inventory risk. A toy backtest that ignores exchange mechanics can lead to fragile strategies and misleading PnL expectations.

## What problem this solves
Market-making strategies often fail when backtests ignore realistic order book mechanics such as queue position, latency, cancellations, partial fills, adverse selection, and inventory risk. OpenLOB aims to provide a deterministic C++ research lab for testing strategies under realistic microstructure constraints before any live deployment.

## Architecture
```text
Agents
  -> Simulation Scheduler
  -> Matching Engine
  -> L3 Order Book
  -> Trades / Acks / Market Data
  -> Metrics / PnL / Inventory
```

## Phase 1 scope
- C++20 skeleton
- deterministic event queue
- L3 order book model
- price-time priority matching
- basic market maker and noise trader agents
- inventory and PnL tracking
- reproducible simulation runs

## Future roadmap
- queue position model
- latency model
- adverse selection detector
- RL quote policy
- Python training bridge
- ONNX/C++ inference runtime
- multi-symbol simulation
- multithreaded batch experiments
- FPGA/Verilog pre-trade risk gate simulator
- OCaml strategy/scenario DSL

## Build instructions
```bash
cmake -S . -B build
cmake --build build
```

Optional:
```bash
cmake -S . -B build -DOPENLOB_BUILD_TESTS=ON -DOPENLOB_BUILD_BENCHMARKS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Current status
The repository currently provides a compile-ready skeleton with placeholder modules, minimal smoke tests, benchmark scaffolding, and foundational documentation. Full matching logic and detailed market microstructure behavior are intentionally deferred to subsequent phases.

## License
MIT License. See `LICENSE`.
