# Market Microstructure Notes

## Limit Order Book
A limit order book (LOB) is a price-organized queue of buy and sell interest. Buy orders form bids; sell orders form asks.

## Bid / Ask
- Best bid: highest resting buy price.
- Best ask: lowest resting sell price.

The difference is the spread, a key driver of market maker economics.

## Spread
The spread reflects liquidity, uncertainty, and competition. Narrow spreads can increase fill rates but may increase adverse-selection risk.

## Market Order vs Limit Order
- Market order: executes immediately against resting liquidity at best available prices.
- Limit order: rests at a chosen price and executes only when matched.

## Price-Time Priority
Most venues use price-time priority:
1. Better price executes first.
2. For equal price, earlier order timestamp executes first.

## Queue Position
Queue position is a major determinant of fill probability for passive quotes. Accurate simulation requires tracking per-price FIFO state.

## Adverse Selection
Adverse selection occurs when fills are concentrated in states where informed flow predicts unfavorable future price movement.

## Inventory Risk
Market makers accumulate inventory through fills. If inventory drifts too far from neutral, PnL volatility and liquidation risk increase.

## Why Naive Backtests Fail
Naive models often ignore queue position, cancellation dynamics, latency, partial fills, and toxic flow. This can materially overestimate strategy quality and stability.
