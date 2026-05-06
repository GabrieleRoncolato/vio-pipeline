#include "frontend/selection/selector_factory.h"
#include "frontend/selection/bucketer_selector.h"
#include "utils/overloaded.h"
#include <variant>

std::unique_ptr<Selector> make_selector(const SelectorConfig& cfg) {
    return std::visit(overloaded {
        [](const BucketerConfig& c) -> std::unique_ptr<Selector> {
            return std::make_unique<BucketerSelector>(c);
        },
    }, cfg);
}