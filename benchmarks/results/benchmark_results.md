# HashMapPro Benchmark Results

## Clear

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Clear empty | 10K | 2.86 ms | 10.77 us | -99.6% |
| Clear empty | 100K | 16.92 ms | 108.31 us | -99.4% |
| Clear empty | 1M | 150.12 ms | 1.08 ms | -99.3% |
| Clear populated | 10K | 103.86 ms | 101.63 ms | -2.1% |
| Clear populated | 100K | 1.05 s | 1.02 s | -2.7% |
| Clear populated | 1M | 10.38 s | 14.57 s | +40.4% |

## Erase

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Erase existing | 10K | 162.89 ms | 127.37 ms | -21.8% |
| Erase existing | 100K | 1.07 s | 1.02 s | -5.0% |
| Erase existing | 1M | 12.22 s | 15.06 s | +23.3% |
| Erase missing | 10K | 219.43 ms | 225.71 ms | +2.9% |
| Erase missing | 100K | 2.19 s | 2.21 s | +0.5% |
| Erase missing | 1M | 16.39 s | 10.72 s | -34.6% |

## Insert

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Insert new | 10K | 103.16 ms | 101.30 ms | -1.8% |
| Insert new | 100K | 1.03 s | 1.01 s | -1.8% |
| Insert new | 1M | 12.12 s | 12.31 s | +1.5% |
| Insert existing | 10K | 191.08 us | 234.38 us | +22.7% |
| Insert existing | 100K | 1.96 ms | 2.34 ms | +19.3% |
| Insert existing | 1M | 19.23 ms | 23.52 ms | +22.4% |

## Update

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Update existing | 10K | 99.00 us | 142.23 us | +43.7% |
| Update existing | 100K | 985.23 us | 1.46 ms | +48.4% |
| Update existing | 1M | 9.90 ms | 14.25 ms | +43.8% |
| Update missing | 10K | 111.39 us | 179.15 us | +60.8% |
| Update missing | 100K | 1.11 ms | 1.83 ms | +64.9% |
| Update missing | 1M | 11.17 ms | 18.10 ms | +62.1% |
