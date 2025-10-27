#include <fmt/core.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

int main() {
    fmt::print("Hello {}\n", "World!");

    boost::uuids::random_generator gen;
    boost::uuids::uuid uid = gen();

    auto str = boost::uuids::to_string(uid);
    fmt::print("Some uuid: {}\n", str);

    return 0;
}