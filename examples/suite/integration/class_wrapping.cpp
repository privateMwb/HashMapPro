// HashMap class wrapping example.
//
// Demonstrates:
// - embedding a HashMap as a private member of a user-defined class
// - exposing a narrow, purpose-built public interface around it
// - the wrapper class benefiting from HashMap's own copy/move semantics

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

// A simple in-memory cache keyed by string, wrapping a HashMap and
// exposing only the operations relevant to callers.
class SimpleCache {
  public:
    explicit SimpleCache(std::size_t capacity) : store_{capacity} {}

    void put(const std::string& key, const std::string& value) {
        store_[key] = value;
    }

    bool tryGet(const std::string& key, std::string& outValue) const {
        auto it = store_.find(key);

        if (it == store_.end())
            return false;

        outValue = it->value;
        return true;
    }

    bool remove(const std::string& key) {
        return store_.erase(key);
    }

    [[nodiscard]] std::size_t entryCount() const {
        return store_.size();
    }

  private:
    HashMap<std::string, std::string> store_;
};

static void run_examples() {
    // Use the wrapper's narrow interface instead of the raw HashMap API.
    setTitle("Using the Wrapper");

    SimpleCache cache{16};

    cache.put("host", "localhost");
    cache.put("port", "8080");

    std::cout << "Entry count : " << cache.entryCount() << "\n\n";

    // Look up a value through the wrapper.
    setTitle("Lookup Through Wrapper");

    std::string value;

    if (cache.tryGet("host", value))
        std::cout << "host -> " << value << "\n";

    if (!cache.tryGet("missing", value))
        std::cout << "\"missing\" not found\n\n";

    // Remove an entry through the wrapper.
    setTitle("Removal Through Wrapper");

    bool removed = cache.remove("port");

    std::cout << "Removed \"port\" : " << removed << "\n";
    std::cout << "Entry count     : " << cache.entryCount() << "\n\n";

    // The wrapper class copies just like the HashMap it holds.
    setTitle("Wrapper Copy Semantics");

    SimpleCache copy = cache;
    copy.put("host", "127.0.0.1");

    std::string original;
    cache.tryGet("host", original);

    std::string copied;
    copy.tryGet("host", copied);

    std::cout << "Original host : " << original << "\n";
    std::cout << "Copy host     : " << copied << "\n";
}

REGISTER_EXAMPLE_SUITE();
