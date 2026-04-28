#pragma once

#include <optional>
#include "types/frame.h"

class CameraReader {
public:
    virtual ~CameraReader() = default;
    virtual std::optional<Frame> read_next() = 0;
    virtual bool is_open() const = 0;
};