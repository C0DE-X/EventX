# EventX

A simple c++11 template based eventsystem.


## Usage
It uses a eventbus for handling events. Multiple eventbusses can be created in multiple threads.
To implement a event handling just inhert the eventlistener class and override the onEvent method.
Events can be triggered by pushing an instance to the eventbus the event should be handled by.
If an event gets pushed also all inheritated eventclasses are getting triggered.


## Creating own event eventclass

This is a short example for a simple user defined event:
```cpp
class CustomEvent : public eventX::Event {
public:
  CustomEvent() : eventX::Event() {}
  ~CustomEvent() override = default;

  EVENTTYPE(CustomEvent, eventX::Event)
};
```
The eventX::Event class must be inheritated and the getType always needs to return its parent type.
Use the *Type::fromEvent(this)* static function to determine your user defined event's type.

## Creating own eventlistener

This is a short example for writing a eventlistener. The following code shows an custom event listener,
that listens to two event types (*eventX::Event, CustomEvent*).

```cpp
class Listener : public eventX::EventListener<eventX::Event, CustomEvent> {
public:
  Listener(eventX::EventBus* bus)
      : eventX::EventListener<eventX::Event, CustomEvent>(bus) {}
  ~Listener() = default;

  void onEvent(std::shared_ptr<eventX::Event> event) override {
    // do something with eventX::Event
  }
  void onEvent(std::shared_ptr<CustomEvent> event) override {
    // do something with CustomEvent
  }
};
```

The eventbus pointer sets the bus on which the listener receives its events.


## Using LocalEventListener

A LocalEventListener can be used to listen eventtypes without inheritance e.g.
for temporary event listening.

```cpp

eventX::LocalEventListener<TestEvent> ll(&eventBus);

ll.setEventCall([](std::shared_ptr<CustomEvent> event) {
  // do something with CustomEvent
});

```

## Running an EventBus

The eventbus class is threadsafe and can be created or run in the main or any thread.
There are two different ways to trigger the eventhandling.

### use own eventloop

```cpp
eventX::EventBus eventBus;
eventBus.exec();
```
The exec function is blocking and the build in eventloop. It runs as long as stop is being called.

### use any other loop and trigger the handling manually

```cpp
eventX::EventBus eventBus;
eventBus.processEvent(); //eventBus.processEvent(short maxCount);
```

The processEvent function handles one event on the queue. The parameter maxCount is optional and
if it is greater then 1, up to the given maxCount events get processed if available on the queue.

### trigger event

```cpp
eventX::EventBus eventBus;
eventBus.push(std::make_shared<CustomEvent>());
```

Simply push a shared_ptr of the desired event type on the eventbus and the event gets processed
by the bus as soon as the eventbus queue reaches this object.
Since CustomEvent inherit eventX::Event, all eventlistener that listen to CustomEvent or/and eventx:Event
are getting called.
