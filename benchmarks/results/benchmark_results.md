# HashMapPro Benchmark Results

## Element Access

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Subscript existing | 10K | 596.00 us | 1.93 ms | +224.1% |
| Subscript existing | 100K | 4.90 ms | 23.62 ms | +381.6% |
| Subscript existing | 1M | 50.23 ms | 189.07 ms | +276.4% |
| Subscript missing | 10K | 3.93 ms | 9.90 ms | +151.7% |
| Subscript missing | 100K | 38.96 ms | 119.11 ms | +205.7% |
| Subscript missing | 1M | 884.73 ms | 1.87 s | +111.1% |
| At existing | 10K | 413.23 us | 1.95 ms | +371.3% |
| At existing | 100K | 4.19 ms | 19.76 ms | +371.0% |
| At existing | 1M | 42.30 ms | 202.57 ms | +378.9% |

## Lookup

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Find hit | 10K | 521.54 us | 1.24 ms | +137.9% |
| Find hit | 100K | 5.27 ms | 12.67 ms | +140.6% |
| Find hit | 1M | 53.12 ms | 129.21 ms | +143.2% |
| Find miss | 10K | 829.23 us | 1.55 ms | +86.3% |
| Find miss | 100K | 6.85 ms | 14.41 ms | +110.3% |
| Find miss | 1M | 58.47 ms | 141.98 ms | +142.8% |
| Contains hit | 10K | 423.46 us | 1.40 ms | +230.2% |
| Contains hit | 100K | 4.22 ms | 14.11 ms | +234.5% |
| Contains hit | 1M | 44.14 ms | 144.68 ms | +227.8% |
| Contains miss | 10K | 439.38 us | 1.56 ms | +254.8% |
| Contains miss | 100K | 4.43 ms | 15.62 ms | +252.8% |
| Contains miss | 1M | 45.65 ms | 158.85 ms | +248.0% |

## Clear

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Clear empty | 10K | 10.66 ms | 169.85 us | -98.4% |
| Clear empty | 100K | 106.33 ms | 1.64 ms | -98.5% |
| Clear empty | 1M | 1.07 s | 19.16 ms | -98.2% |
| Clear populated | 10K | 164.91 ms | 399.83 ms | +142.4% |
| Clear populated | 100K | 1.65 s | 3.99 s | +141.8% |
| Clear populated | 1M | 32.15 s | 81.02 s | +152.0% |

## Erase

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Erase existing | 10K | 328.17 ms | 799.62 ms | +143.7% |
| Erase existing | 100K | 3.28 s | 8.08 s | +146.5% |
| Erase existing | 1M | 151.15 s | 84.16 s | -44.3% |
| Erase missing | 10K | 406.91 ms | 922.71 ms | +126.8% |
| Erase missing | 100K | 3.21 s | 7.59 s | +136.2% |
| Erase missing | 1M | 33.62 s | 77.08 s | +129.3% |

## Insert

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Insert new | 10K | 319.19 ms | 784.49 ms | +145.8% |
| Insert new | 100K | 3.23 s | 7.88 s | +144.1% |
| Insert new | 1M | 32.83 s | 76.76 s | +133.8% |
| Insert existing | 10K | 836.00 us | 2.86 ms | +242.5% |
| Insert existing | 100K | 8.38 ms | 28.44 ms | +239.4% |
| Insert existing | 1M | 84.08 ms | 284.47 ms | +238.3% |

## Update

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Update existing | 10K | 3.40 ms | 3.41 ms | +0.3% |
| Update existing | 100K | 8.81 ms | 34.86 ms | +295.6% |
| Update existing | 1M | 89.25 ms | 350.25 ms | +292.4% |
| Update missing | 10K | 2.06 ms | 4.98 ms | +141.9% |
| Update missing | 100K | 17.10 ms | 30.14 ms | +76.2% |
| Update missing | 1M | 90.38 ms | 294.48 ms | +225.8% |

## Constructor

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Default construct | 10K | 8.82 ms | 1.20 ms | -86.4% |
| Default construct | 100K | 87.41 ms | 12.06 ms | -86.2% |
| Default construct | 1M | 877.61 ms | 127.48 ms | -85.5% |
| Reserved construct | 10K | 38.13 ms | 25.15 ms | -34.0% |
| Reserved construct | 100K | 377.69 ms | 251.14 ms | -33.5% |
| Reserved construct | 1M | 3.79 s | 2.65 s | -30.2% |
| Populate construct | 10K | 29.29 ms | 76.13 ms | +159.9% |
| Populate construct | 100K | 307.07 ms | 874.39 ms | +184.8% |
| Populate construct | 1M | 3.22 s | 8.07 s | +150.9% |

## Copy

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Copy construct | 10K | 546.98 ms | 1.52 s | +178.7% |
| Copy construct | 100K | 5.93 s | 14.51 s | +144.8% |
| Copy construct | 1M | 51.76 s | 125.64 s | +142.8% |
| Copy assignment | 10K | 492.02 ms | 486.91 ms | -1.0% |
| Copy assignment | 100K | 4.90 s | 4.90 s | -0.0% |
| Copy assignment | 1M | 49.66 s | 57.59 s | +16.0% |

## Destructor

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Destroy empty | 10K | 50.22 ms | 78.48 ms | +56.3% |
| Destroy empty | 100K | 771.35 ms | 607.71 ms | -21.2% |
| Destroy empty | 1M | 5.16 s | 4.42 s | -14.4% |
| Destroy populated | 10K | 924.52 ms | 1.98 s | +114.4% |
| Destroy populated | 100K | 6.37 s | 11.90 s | +86.7% |
| Destroy populated | 1M | 63.56 s | 128.66 s | +102.4% |

## Move

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Move construct | 10K | 568.81 ms | 1.17 s | +106.3% |
| Move construct | 100K | 5.83 s | 11.95 s | +104.9% |
| Move construct | 1M | 59.14 s | 122.35 s | +106.9% |
| Move assignment | 10K | 579.93 ms | 1.19 s | +105.8% |
| Move assignment | 100K | 5.94 s | 12.25 s | +106.1% |
| Move assignment | 1M | 59.52 s | 126.57 s | +112.7% |

## Load Factor

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Lookup sparse | 10K | 1.38 ms | 2.37 ms | +72.0% |
| Lookup sparse | 100K | 9.91 ms | 23.32 ms | +135.4% |
| Lookup sparse | 1M | 96.29 ms | 242.81 ms | +152.2% |
| Lookup dense | 10K | 1.11 ms | 2.27 ms | +104.4% |
| Lookup dense | 100K | 9.84 ms | 23.96 ms | +143.5% |
| Lookup dense | 1M | 94.01 ms | 236.69 ms | +151.8% |

## Rehash

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Rehash trigger | 10K | 584.23 ms | 1.48 s | +153.3% |
| Rehash trigger | 100K | 5.89 s | 13.39 s | +127.2% |
| Rehash trigger | 1M | 58.42 s | 131.30 s | +124.7% |
| No rehash baseline | 10K | 547.75 ms | 1.27 s | +132.4% |
| No rehash baseline | 100K | 5.45 s | 12.79 s | +134.9% |
| No rehash baseline | 1M | 54.72 s | 122.26 s | +123.4% |

## Reserve

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Reserve Call | 10K | 133.03 ms | 48.34 ms | -63.7% |
| Reserve Call | 100K | 1.33 s | 503.40 ms | -62.1% |
| Reserve Call | 1M | 13.61 s | 5.23 s | -61.6% |
| Insert No Reserve | 10K | 330.60 ms | 743.71 ms | +125.0% |
| Insert No Reserve | 100K | 3.58 s | 7.15 s | +99.9% |
| Insert No Reserve | 1M | 34.09 s | 76.28 s | +123.8% |
| Insert With Reserve | 10K | 1.09 s | 1.24 s | +13.6% |
| Insert With Reserve | 100K | 5.55 s | 6.43 s | +15.9% |
| Insert With Reserve | 1M | 26.94 s | 63.83 s | +136.9% |

## Capacity

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Size | 10K | 1.59 ms | 341.00 us | -78.5% |
| Size | 100K | 3.70 ms | 3.58 ms | -3.2% |
| Size | 1M | 33.21 ms | 42.42 ms | +27.7% |
| Capacity | 10K | 274.69 us | 960.92 us | +249.8% |
| Capacity | 100K | 2.81 ms | 4.80 ms | +70.6% |
| Capacity | 1M | 35.23 ms | 43.79 ms | +24.3% |
| Empty | 10K | 300.92 us | 347.31 us | +15.4% |
| Empty | 100K | 3.07 ms | 5.68 ms | +84.8% |
| Empty | 1M | 31.44 ms | 37.30 ms | +18.7% |

## Iteration

| Test | Iteration | HashMapPro | std::unordered_map | Δ |
|---|---|---|---|---|
| Forward traverse | 10K | 63.81 ms | 72.73 ms | +14.0% |
| Forward traverse | 100K | 627.56 ms | 769.50 ms | +22.6% |
| Forward traverse | 1M | 6.45 s | 7.60 s | +17.7% |
| Reverse traverse | 10K | 177.23 ms | 74.37 ms | -58.0% |
| Reverse traverse | 100K | 1.70 s | 774.98 ms | -54.5% |
| Reverse traverse | 1M | 17.24 s | 7.60 s | -55.9% |
