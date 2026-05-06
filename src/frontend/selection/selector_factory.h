#pragma once

#include "types/config.h"
#include "frontend/selection/selector.h"
#include <memory>

std::unique_ptr<Selector> make_selector(const SelectorConfig& cfg);