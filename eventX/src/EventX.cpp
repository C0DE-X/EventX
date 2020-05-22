#include <EventX.h>
#include <thread>
#include <iostream>

void EventX::exec()
{
    m_x.store(true);

    int i = 0;
    while(m_x && i < 1000)
    {
        //std::this_thread::yield();
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::cout<<"Eventsystem running\n";
        ++i;
    }
}

void EventX::stop()
{
    m_x.store(false);
}
