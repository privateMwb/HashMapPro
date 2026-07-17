# HashMapPro Regression Report

## Element Access

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| subscript existing | 10K | 2.73 ns | 12.12 ns | +344.0% |
| subscript existing | 100K | 2.82 ns | 12.09 ns | +328.7% |
| subscript existing | 1M | 2.35 ns | 11.93 ns | +407.7% |
| subscript missing | 10K | 32.12 ns | 519.02 ns | +1515.9% |
| subscript missing | 100K | 32.68 ns | 520.97 ns | +1494.2% |
| subscript missing | 1M | 32.12 ns | 649.77 ns | +1922.9% |
| at existing | 10K | 0.72 ns | 10.03 ns | +1293.1% |
| at existing | 100K | 0.93 ns | 10.03 ns | +978.5% |
| at existing | 1M | 0.65 ns | 18.39 ns | +2729.2% |

## Lookup

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| find hit | 10K | 1.24 ns | 10.06 ns | +711.3% |
| find hit | 100K | 1.24 ns | 10.05 ns | +710.5% |
| find hit | 1M | 1.24 ns | 13.11 ns | +957.3% |
| find miss | 10K | 2.17 ns | 255.56 ns | +11677.0% |
| find miss | 100K | 2.16 ns | 16.26 ns | +652.8% |
| find miss | 1M | 2.21 ns | 22.94 ns | +938.0% |
| contains hit | 10K | 0.93 ns | 16.8 ns | +1706.5% |
| contains hit | 100K | 0.62 ns | 15.01 ns | +2321.0% |
| contains hit | 1M | 0.93 ns | 10.26 ns | +1003.2% |
| contains miss | 10K | 1.24 ns | 12.03 ns | +870.2% |
| contains miss | 100K | 1.54 ns | 12 ns | +679.2% |
| contains miss | 1M | 1.55 ns | 12.19 ns | +686.5% |

## Clear

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| clear empty | 10K | 125.33 ns | 370.43 ns | +195.6% |
| clear empty | 100K | 85.17 ns | 363.82 ns | +327.2% |
| clear empty | 1M | 125.15 ns | 458.64 ns | +266.5% |
| clear populated | 10K | 1390.94 ns | 24019.9 ns | +1626.9% |
| clear populated | 100K | 1275.75 ns | 33869.2 ns | +2554.8% |
| clear populated | 1M | 1292.79 ns | 31075.5 ns | +2303.8% |

## Erase

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| erase existing | 10K | 1359.05 ns | 50825.2 ns | +3639.8% |
| erase existing | 100K | 1332.38 ns | 51619.7 ns | +3774.3% |
| erase existing | 1M | 1351.74 ns | 30633 ns | +2166.2% |
| erase missing | 10K | 1360.83 ns | 24291.2 ns | +1685.0% |
| erase missing | 100K | 1313.02 ns | 24587.1 ns | +1772.6% |
| erase missing | 1M | 1333.36 ns | 26611.2 ns | +1895.8% |

## Insert

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| insert new | 10K | 1309.72 ns | 24732.2 ns | +1788.4% |
| insert new | 100K | 1332.29 ns | 25677.6 ns | +1827.3% |
| insert new | 1M | 1315.01 ns | 29051.6 ns | +2109.2% |
| insert existing | 10K | 2.28 ns | 21.41 ns | +839.0% |
| insert existing | 100K | 2.56 ns | 22.16 ns | +765.6% |
| insert existing | 1M | 2.29 ns | 22.95 ns | +902.2% |

## Update

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| update existing | 10K | 0.93 ns | 11.08 ns | +1091.4% |
| update existing | 100K | 0.93 ns | 11.04 ns | +1087.1% |
| update existing | 1M | 0.82 ns | 11.35 ns | +1284.1% |
| update missing | 10K | 1.86 ns | 12.46 ns | +569.9% |
| update missing | 100K | 1.54 ns | 12.42 ns | +706.5% |
| update missing | 1M | 1.56 ns | 12.74 ns | +716.7% |

## Constructor

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| default construct | 10K | 38.65 ns | 410.13 ns | +961.1% |
| default construct | 100K | 37.5 ns | 413.55 ns | +1002.8% |
| default construct | 1M | 36.88 ns | 410.85 ns | +1014.0% |
| reserved construct | 10K | 149.72 ns | 1087.42 ns | +626.3% |
| reserved construct | 100K | 149.47 ns | 1061.44 ns | +610.1% |
| reserved construct | 1M | 149.74 ns | 1067.6 ns | +613.0% |
| populate construct | 10K | 106.99 ns | 2071.48 ns | +1836.1% |
| populate construct | 100K | 107.68 ns | 2082.77 ns | +1834.2% |
| populate construct | 1M | 106.53 ns | 2124.16 ns | +1894.0% |

## Copy

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| copy construct | 10K | 2807.19 ns | 39151.9 ns | +1294.7% |
| copy construct | 100K | 2849.4 ns | 38094.5 ns | +1236.9% |
| copy construct | 1M | 2844.17 ns | 38302 ns | +1246.7% |
| copy assignment | 10K | 2650.99 ns | 37088.9 ns | +1299.1% |
| copy assignment | 100K | 2568.82 ns | 38510.1 ns | +1399.1% |
| copy assignment | 1M | 2648.73 ns | 42818.3 ns | +1516.6% |

## Destructor

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| destroy empty | 10K | 196.32 ns | 1067.78 ns | +443.9% |
| destroy empty | 100K | 150.48 ns | 1071.97 ns | +612.4% |
| destroy empty | 1M | 150.17 ns | 1135.54 ns | +656.2% |
| destroy populated | 10K | 2082.02 ns | 39866.3 ns | +1814.8% |
| destroy populated | 100K | 2093.28 ns | 40746.5 ns | +1846.5% |
| destroy populated | 1M | 2061.2 ns | 40787.4 ns | +1878.8% |

## Move

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| move construct | 10K | 2242.81 ns | 38314.3 ns | +1608.3% |
| move construct | 100K | 2273.21 ns | 39360.3 ns | +1631.5% |
| move construct | 1M | 2324.04 ns | 39640.2 ns | +1605.7% |
| move assignment | 10K | 2388.91 ns | 40833.8 ns | +1609.3% |
| move assignment | 100K | 2309.96 ns | 40809.9 ns | +1666.7% |
| move assignment | 1M | 2312.18 ns | 40710 ns | +1660.7% |

## Load Factor

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| lookup sparse | 10K | 1.24 ns | 10.01 ns | +707.3% |
| lookup sparse | 100K | 1.38 ns | 26.47 ns | +1818.1% |
| lookup sparse | 1M | 1.25 ns | 10.16 ns | +712.8% |
| lookup dense | 10K | 1.24 ns | 10.03 ns | +708.9% |
| lookup dense | 100K | 1.24 ns | 10.59 ns | +754.0% |
| lookup dense | 1M | 1.24 ns | 13.82 ns | +1014.5% |

## Rehash

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| rehash trigger | 10K | 2261.24 ns | 49835.4 ns | +2103.9% |
| rehash trigger | 100K | 2270.61 ns | 57966.5 ns | +2452.9% |
| rehash trigger | 1M | 2248.89 ns | 41268.1 ns | +1735.0% |
| no rehash baseline | 10K | 3828.5 ns | 36915.7 ns | +864.2% |
| no rehash baseline | 100K | 3803.04 ns | 37426.6 ns | +884.1% |
| no rehash baseline | 1M | 3808.49 ns | 37451.5 ns | +883.4% |

## Reserve

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| reserve_call | 10K | 441.12 ns | 3407.05 ns | +672.4% |
| reserve_call | 100K | 441.54 ns | 3354.98 ns | +659.8% |
| reserve_call | 1M | 441.84 ns | 3400.79 ns | +669.7% |
| insert_no_reserve | 10K | 1256.27 ns | 21278.7 ns | +1593.8% |
| insert_no_reserve | 100K | 1201.75 ns | 21811.5 ns | +1715.0% |
| insert_no_reserve | 1M | 1238.93 ns | 22603.7 ns | +1724.5% |
| insert_with_reserve | 10K | 1038.35 ns | 18440.4 ns | +1675.9% |
| insert_with_reserve | 100K | 1064.06 ns | 18939.3 ns | +1679.9% |
| insert_with_reserve | 1M | 931.39 ns | 18804.8 ns | +1919.0% |

## Capacity

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| size | 10K | 0.32 ns | 1.35 ns | +321.9% |
| size | 100K | 0.31 ns | 2 ns | +545.2% |
| size | 1M | 0.32 ns | 2 ns | +525.0% |
| capacity | 10K | 0.31 ns | 1.33 ns | +329.0% |
| capacity | 100K | 0.31 ns | 1.34 ns | +332.3% |
| capacity | 1M | 0.31 ns | 1.33 ns | +329.0% |
| empty | 10K | 0.31 ns | 2.68 ns | +764.5% |
| empty | 100K | 0.32 ns | 2.67 ns | +734.4% |
| empty | 1M | 0.31 ns | 2.75 ns | +787.1% |

## Iteration

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| forward traverse | 10K | 61.07 ns | 723.82 ns | +1085.2% |
| forward traverse | 100K | 61.87 ns | 639.12 ns | +933.0% |
| forward traverse | 1M | 85.93 ns | 632.8 ns | +636.4% |
| reverse traverse | 10K | 260.68 ns | 1351.65 ns | +418.5% |
| reverse traverse | 100K | 240.61 ns | 1310.69 ns | +444.7% |
| reverse traverse | 1M | 221.82 ns | 1276.25 ns | +475.4% |
