#ifndef EVENTX_H
#define EVENTX_H

#include <EventX_globals.h>
#include <atomic>

class EVENTX_EXPORT EventX
{
public:
    EventX() = default;
    ~EventX() = default;

    void exec();
    void stop();
    void test();

private:
    std::atomic_bool m_x { false };
};

#endif // EVENTX_H
