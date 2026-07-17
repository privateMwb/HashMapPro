# HashMapPro Benchmark Results

## Element Access

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Subscript existing | 10K | 121.23 us | 175.69 us | +44.9% |
| Subscript existing | 100K | 1.21 ms | 1.74 ms | +43.5% |
| Subscript existing | 1M | 11.93 ms | 17.14 ms | +43.7% |
| Subscript missing | 10K | 5.19 ms | 7.98 ms | +53.8% |
| Subscript missing | 100K | 52.10 ms | 65.15 ms | +25.0% |
| Subscript missing | 1M | 649.77 ms | 1.25 s | +92.0% |
| At existing | 10K | 100.31 us | 154.08 us | +53.6% |
| At existing | 100K | 1.00 ms | 5.19 ms | +417.3% |
| At existing | 1M | 18.39 ms | 19.79 ms | +7.6% |

## Lookup

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Find hit | 10K | 100.62 us | 147.31 us | +46.4% |
| Find hit | 100K | 1.01 ms | 8.05 ms | +700.7% |
| Find hit | 1M | 13.11 ms | 16.45 ms | +25.5% |
| Find miss | 10K | 2.56 ms | 195.77 us | -92.3% |
| Find miss | 100K | 1.63 ms | 2.00 ms | +23.1% |
| Find miss | 1M | 22.94 ms | 22.53 ms | -1.8% |
| Contains hit | 10K | 168.00 us | 153.92 us | -8.4% |
| Contains hit | 100K | 1.50 ms | 2.04 ms | +35.8% |
| Contains hit | 1M | 10.26 ms | 16.30 ms | +58.9% |
| Contains miss | 10K | 120.31 us | 187.62 us | +55.9% |
| Contains miss | 100K | 1.20 ms | 1.93 ms | +60.8% |
| Contains miss | 1M | 12.19 ms | 19.25 ms | +57.9% |

## Clear

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Clear empty | 10K | 3.70 ms | 27.00 us | -99.3% |
| Clear empty | 100K | 36.38 ms | 266.77 us | -99.3% |
| Clear empty | 1M | 458.64 ms | 2.67 ms | -99.4% |
| Clear populated | 10K | 240.20 ms | 235.26 ms | -2.1% |
| Clear populated | 100K | 3.39 s | 4.10 s | +21.0% |
| Clear populated | 1M | 31.08 s | 33.15 s | +6.7% |

## Erase

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Erase existing | 10K | 508.25 ms | 609.52 ms | +19.9% |
| Erase existing | 100K | 5.16 s | 3.80 s | -26.4% |
| Erase existing | 1M | 30.63 s | 28.16 s | -8.1% |
| Erase missing | 10K | 242.91 ms | 247.74 ms | +2.0% |
| Erase missing | 100K | 2.46 s | 2.46 s | -0.1% |
| Erase missing | 1M | 26.61 s | 25.95 s | -2.5% |

## Insert

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Insert new | 10K | 247.32 ms | 244.01 ms | -1.3% |
| Insert new | 100K | 2.57 s | 2.54 s | -0.9% |
| Insert new | 1M | 29.05 s | 26.11 s | -10.1% |
| Insert existing | 10K | 214.08 us | 262.69 us | +22.7% |
| Insert existing | 100K | 2.22 ms | 2.85 ms | +28.4% |
| Insert existing | 1M | 22.95 ms | 26.60 ms | +15.9% |

## Update

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Update existing | 10K | 110.77 us | 159.31 us | +43.8% |
| Update existing | 100K | 1.10 ms | 1.68 ms | +52.5% |
| Update existing | 1M | 11.35 ms | 16.05 ms | +41.4% |
| Update missing | 10K | 124.62 us | 200.85 us | +61.2% |
| Update missing | 100K | 1.24 ms | 2.08 ms | +67.5% |
| Update missing | 1M | 12.74 ms | 20.25 ms | +58.9% |

## Constructor

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Default construct | 10K | 4.10 ms | 69.23 us | -98.3% |
| Default construct | 100K | 41.35 ms | 753.92 us | -98.2% |
| Default construct | 1M | 410.85 ms | 6.99 ms | -98.3% |
| Reserved construct | 10K | 10.87 ms | 5.68 ms | -47.7% |
| Reserved construct | 100K | 106.14 ms | 57.41 ms | -45.9% |
| Reserved construct | 1M | 1.07 s | 575.49 ms | -46.1% |
| Populate construct | 10K | 20.71 ms | 26.58 ms | +28.3% |
| Populate construct | 100K | 208.28 ms | 258.65 ms | +24.2% |
| Populate construct | 1M | 2.12 s | 2.63 s | +24.0% |

## Copy

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Copy construct | 10K | 391.52 ms | 412.31 ms | +5.3% |
| Copy construct | 100K | 3.81 s | 4.01 s | +5.2% |
| Copy construct | 1M | 38.30 s | 41.02 s | +7.1% |
| Copy assignment | 10K | 370.89 ms | 53.51 ms | -85.6% |
| Copy assignment | 100K | 3.85 s | 666.90 ms | -82.7% |
| Copy assignment | 1M | 42.82 s | 5.56 s | -87.0% |

## Destructor

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Destroy empty | 10K | 10.68 ms | 5.09 ms | -52.3% |
| Destroy empty | 100K | 107.20 ms | 51.32 ms | -52.1% |
| Destroy empty | 1M | 1.14 s | 517.71 ms | -54.4% |
| Destroy populated | 10K | 398.66 ms | 395.05 ms | -0.9% |
| Destroy populated | 100K | 4.07 s | 4.07 s | -0.2% |
| Destroy populated | 1M | 40.79 s | 38.63 s | -5.3% |

## Move

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Move construct | 10K | 383.14 ms | 377.40 ms | -1.5% |
| Move construct | 100K | 3.94 s | 3.77 s | -4.3% |
| Move construct | 1M | 39.64 s | 38.14 s | -3.8% |
| Move assignment | 10K | 408.34 ms | 382.93 ms | -6.2% |
| Move assignment | 100K | 4.08 s | 3.82 s | -6.4% |
| Move assignment | 1M | 40.71 s | 38.41 s | -5.6% |

## Load Factor

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Lookup sparse | 10K | 100.08 us | 147.31 us | +47.2% |
| Lookup sparse | 100K | 2.65 ms | 2.95 ms | +11.3% |
| Lookup sparse | 1M | 10.16 ms | 26.27 ms | +158.6% |
| Lookup dense | 10K | 100.31 us | 190.54 us | +90.0% |
| Lookup dense | 100K | 1.06 ms | 1.47 ms | +38.7% |
| Lookup dense | 1M | 13.82 ms | 18.55 ms | +34.3% |

## Rehash

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Rehash trigger | 10K | 498.35 ms | 633.08 ms | +27.0% |
| Rehash trigger | 100K | 5.80 s | 4.43 s | -23.6% |
| Rehash trigger | 1M | 41.27 s | 41.68 s | +1.0% |
| No rehash baseline | 10K | 369.16 ms | 382.92 ms | +3.7% |
| No rehash baseline | 100K | 3.74 s | 3.77 s | +0.7% |
| No rehash baseline | 1M | 37.45 s | 37.92 s | +1.3% |

## Reserve

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Reserve Call | 10K | 34.07 ms | 6.24 ms | -81.7% |
| Reserve Call | 100K | 335.50 ms | 62.84 ms | -81.3% |
| Reserve Call | 1M | 3.40 s | 628.44 ms | -81.5% |
| Insert No Reserve | 10K | 212.79 ms | 220.82 ms | +3.8% |
| Insert No Reserve | 100K | 2.18 s | 2.18 s | -0.1% |
| Insert No Reserve | 1M | 22.60 s | 22.12 s | -2.1% |
| Insert With Reserve | 10K | 184.40 ms | 182.92 ms | -0.8% |
| Insert With Reserve | 100K | 1.89 s | 1.88 s | -0.8% |
| Insert With Reserve | 1M | 18.80 s | 18.79 s | -0.1% |

## Capacity

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Size | 10K | 13.54 us | 13.62 us | +0.6% |
| Size | 100K | 200.23 us | 133.54 us | -33.3% |
| Size | 1M | 2.00 ms | 1.40 ms | -29.9% |
| Capacity | 10K | 13.31 us | 20.00 us | +50.3% |
| Capacity | 100K | 133.54 us | 200.08 us | +49.8% |
| Capacity | 1M | 1.33 ms | 2.09 ms | +57.0% |
| Empty | 10K | 26.77 us | 26.85 us | +0.3% |
| Empty | 100K | 266.77 us | 266.77 us | +0.0% |
| Empty | 1M | 2.75 ms | 2.67 ms | -3.0% |

## Iteration

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Forward traverse | 10K | 7.24 ms | 1.99 ms | -72.5% |
| Forward traverse | 100K | 63.91 ms | 17.62 ms | -72.4% |
| Forward traverse | 1M | 632.80 ms | 178.73 ms | -71.8% |
| Reverse traverse | 10K | 13.52 ms | 1.84 ms | -86.4% |
| Reverse traverse | 100K | 131.07 ms | 20.75 ms | -84.2% |
| Reverse traverse | 1M | 1.28 s | 179.25 ms | -86.0% |
