# EventX

A simple c++11 template based eventsystem.


## Usage
It uses a eventbus for handling events. Multiple eventbusses can be created in multiple threads.
To implement a event handling just inhert the eventlistener class and override the onEvent method.
Events can be triggered by pushing an instance to the eventbus the event should be handled.
If an event gets pushed also all inheritated eventclasses are getting triggered.

