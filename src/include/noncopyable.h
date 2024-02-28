#pragma once

namespace mprpc
{
/// @brief private继承后不可被赋值和复制构造
class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};
} // namespace mprpc
