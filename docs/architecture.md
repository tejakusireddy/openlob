# OpenLOB Architecture

OpenLOB is organized into focused modules that separate market data structures, matching behavior, simulation orchestration, agents, and metrics.

## Core Engine
The `core` module defines shared primitives such as side, order types, events, identifiers, and nanosecond timestamps. These are deterministic building blocks used across every subsystem.

## Order Book
The `book` module holds the L3 limit order book representation. In later phases it will implement price-level queues, order-id indexing, and queue-position tracking.

## Matching Engine
The matching engine fronts the order book and will eventually enforce exchange-style price-time priority semantics, partial fills, and market order behavior.

## Simulator
The `sim` module provides a deterministic event queue and a simulation loop that advances logical time and dispatches events. Reproducibility is prioritized over raw throughput in early phases.

## Agents
The `agents` module defines agent interfaces and initial strategy placeholders:
- market maker
- noise trader
- informed trader

## Metrics
The `metrics` module collects risk and performance outputs such as inventory, PnL, and latency statistics. These begin as lightweight placeholders and will grow with execution realism.

## Future AI Layer
AI/RL strategy learning and inference are planned for later phases after order book and simulation correctness are established.
