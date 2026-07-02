#include <iostream>

void run_constructor_benchmarks();
void run_modifiers_benchmarks();
void run_lookup_benchmarks();
void run_access_benchmarks();
void run_iteration_benchmarks();

int main() {
    run_constructor_benchmarks();
    run_modifiers_benchmarks();
    run_lookup_benchmarks();
    run_access_benchmarks();
    run_iteration_benchmarks();

    return 0;
}