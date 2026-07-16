// HashMap Exception-Safe Clone Test Suite
// Verifies that a throwing value type does not leak resources or
// corrupt state when copy construction or copy assignment fails
// partway through cloning elements.
//
// Covers:
// - copy construction that throws mid-clone propagates the exception
// - the source map is untouched after a failed copy construction
// - copy assignment that throws leaves the destination empty, not
//   half-populated or corrupted
// - a destination left empty by a failed assignment remains usable
// - cloning succeeds normally when no throw is configured

#include <common/framework.h>

#include <stdexcept>

// A value type that throws from its copy constructor once a
// configured number of copies have been made, used to force
// cloneFrom() down its exception-handling path.
struct ThrowOnCopy {
    int data;

    static int copyCount;
    static int throwAfter; // -1 means never throw

    ThrowOnCopy() : data(0) {}
    explicit ThrowOnCopy(int d) : data(d) {}

    ThrowOnCopy(const ThrowOnCopy& other) : data(other.data) {
        ++copyCount;

        if (throwAfter >= 0 && copyCount > throwAfter) {
            throw std::runtime_error("ThrowOnCopy: forced copy failure");
        }
    }

    ThrowOnCopy& operator=(const ThrowOnCopy&) = default;
};

int ThrowOnCopy::copyCount = 0;
int ThrowOnCopy::throwAfter = -1;

// Resets the shared throwing-copy counters before each test case, so
// tests remain independent of one another.
static void resetThrowState() {
    ThrowOnCopy::copyCount = 0;
    ThrowOnCopy::throwAfter = -1;
}

// Verifies a copy construction that throws mid-clone propagates the
// exception rather than being silently swallowed.
static void copy_construction_throws_mid_clone() {
    resetThrowState();

    HashMapPro::HashMap<int, ThrowOnCopy> source{8};

    for (int i = 0; i < 5; ++i) {
        source.insert(i, ThrowOnCopy(i * 10));
    }

    ThrowOnCopy::copyCount = 0;
    ThrowOnCopy::throwAfter = 3;

    bool threw = false;

    try {
        HashMapPro::HashMap<int, ThrowOnCopy> copy{source};
    } catch (const std::runtime_error&) {
        threw = true;
    }

    CHK(threw == true);
}

// Verifies the source map is left completely untouched after a
// failed copy construction attempt.
static void source_untouched_after_failed_copy() {
    resetThrowState();

    HashMapPro::HashMap<int, ThrowOnCopy> source{8};

    for (int i = 0; i < 5; ++i) {
        source.insert(i, ThrowOnCopy(i * 10));
    }

    ThrowOnCopy::copyCount = 0;
    ThrowOnCopy::throwAfter = 3;

    try {
        HashMapPro::HashMap<int, ThrowOnCopy> copy{source};
    } catch (const std::runtime_error&) {
        // Expected; source integrity is checked below regardless.
    }

    CHK(source.size() == 5);

    for (int i = 0; i < 5; ++i) {
        CHK(source.at(i).data == i * 10);
    }
}

// Verifies a copy assignment that throws leaves the destination
// empty rather than half-populated, since the destination is
// released before cloning begins and released again on failure.
static void copy_assignment_throws_leaves_destination_empty() {
    resetThrowState();

    HashMapPro::HashMap<int, ThrowOnCopy> source{8};
    source.insert(1, ThrowOnCopy(100));
    source.insert(2, ThrowOnCopy(200));

    HashMapPro::HashMap<int, ThrowOnCopy> destination{8};
    destination.insert(99, ThrowOnCopy(999));

    ThrowOnCopy::copyCount = 0;
    ThrowOnCopy::throwAfter = 0;

    bool threw = false;

    try {
        destination = source;
    } catch (const std::runtime_error&) {
        threw = true;
    }

    CHK(threw == true);
    CHK(destination.size() == 0);
    CHK(destination.empty() == true);

    CHK(source.size() == 2);
    CHK(source.at(1).data == 100);
    CHK(source.at(2).data == 200);
}

// Verifies a destination left empty by a failed assignment remains
// safely usable afterward, rather than being left in a broken state.
static void destination_reusable_after_failed_assignment() {
    resetThrowState();

    HashMapPro::HashMap<int, ThrowOnCopy> source{8};
    source.insert(1, ThrowOnCopy(100));

    HashMapPro::HashMap<int, ThrowOnCopy> destination{8};

    ThrowOnCopy::copyCount = 0;
    ThrowOnCopy::throwAfter = 0;

    try {
        destination = source;
    } catch (const std::runtime_error&) {
        // Expected.
    }

    resetThrowState();

    destination.insert(5, ThrowOnCopy(50));

    CHK(destination.size() == 1);
    CHK(destination.at(5).data == 50);
}

// Verifies cloning still succeeds normally when no throw is
// configured, confirming the throwing type doesn't interfere with
// the ordinary copy path.
static void clone_succeeds_without_configured_throw() {
    resetThrowState();

    HashMapPro::HashMap<int, ThrowOnCopy> source{8};
    source.insert(1, ThrowOnCopy(100));
    source.insert(2, ThrowOnCopy(200));

    HashMapPro::HashMap<int, ThrowOnCopy> copy{source};

    CHK(copy.size() == 2);
    CHK(copy.at(1).data == 100);
    CHK(copy.at(2).data == 200);
}

// Executes all exception-safe clone test cases.
static void run_tests() {
    RUN(copy_construction_throws_mid_clone);
    RUN(source_untouched_after_failed_copy);
    RUN(copy_assignment_throws_leaves_destination_empty);
    RUN(destination_reusable_after_failed_assignment);
    RUN(clone_succeeds_without_configured_throw);
}

REGISTER_TEST_SUITE();
