#include "test_helper.h"

#include <iostream>

void run_constructor_tests();
void run_access_tests();
void run_modifiers_tests();
void run_lookup_tests();
void run_iterator_tests();
void run_capacity_tests();

int main() {
	std::cout << "\n";

	run_constructor_tests();
	run_access_tests();
	run_modifiers_tests();
	run_lookup_tests();
	run_iterator_tests();
	run_capacity_tests();

	stats();
	std::cout << "\n";

	return 0;
}