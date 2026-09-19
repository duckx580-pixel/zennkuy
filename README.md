# Zennkuy Message Queue Implementation

A C++ message queue system for native code that provides thread-safe, producer-consumer pattern implementation based on the PowerKuy architecture from RealGrowlauncherv5.57.

## Features

- **Thread-safe message queues** using `std::queue` and `std::mutex`
- **Two independent queues**:
  1. Main OS message queue - for 40+ message types
  2. PowerKuy soft keyboard queue - for UI keyboard events
- **Non-blocking message retrieval** - returns 0 if queue is empty
- **Proper memory management** - automatic cleanup with standard containers
- **C/C++ compatible** - can be called from both C and C++ code

## Message Types

### Main OS Messages (0-1011)

Core messages:
- `MESSAGE_NONE` (0) - No message
- `MESSAGE_OPEN_TEXT_BOX` (1) - Open text input
- `MESSAGE_CLOSE_TEXT_BOX` (2) - Close text input
- `MESSAGE_CHECK_CONNECTION` (3) - Check network connection
- `MESSAGE_SET_FPS_LIMIT` (4) - Set FPS limit
- `MESSAGE_FINISH_APP` (6) - Close application

And 40+ additional message types for various app functions.

### PowerKuy Soft Keyboard Messages

Handled separately for UI responsiveness:
- `MESSAGE_SHOW_SOFT_KEYBOARD` (1) - Show keyboard
- `MESSAGE_HIDE_SOFT_KEYBOARD` (2) - Hide keyboard
- 0 - No message / queue empty

## API Reference

### Push Messages

```cpp
// Push to main message queue
void nativePushMessageZennKuy(int messageType);

// Push to PowerKuy soft keyboard queue
void nativePushMessagePowerKuy(int messageType);
```

### Get Messages

```cpp
// Get next OS message (0 if empty)
int nativeGetMessageZennKuy();

// Get next PowerKuy keyboard message (0, 1, or 2)
int nativeGetMessagePowerKuy();
```

### Queue Management

```cpp
// Get queue sizes for debugging
int nativeGetMessageQueueSize();
int nativeGetPowerKuyMessageQueueSize();

// Clear all messages (emergency/shutdown)
void nativeEmergencyMessageClear();

// Initialize/shutdown
void nativeInitializeMessageQueue();
void nativeShutdownMessageQueue();
```

## Usage Pattern

```cpp
// Main game loop (from AppRenderer.java pattern)
void gameLoop() {
    // Handle PowerKuy soft keyboard messages
    int powerKuyMsg = nativeGetMessagePowerKuy();
    if (powerKuyMsg != 0) {
        if (powerKuyMsg == 1) {
            // Show soft keyboard
        } else if (powerKuyMsg == 2) {
            // Hide soft keyboard
        }
    }
    
    // Handle OS messages
    int osMsg = nativeGetMessageZennKuy();
    while (osMsg != 0) {
        switch (osMsg) {
            case MESSAGE_OPEN_TEXT_BOX:
                // Handle open text box
                break;
            case MESSAGE_CLOSE_TEXT_BOX:
                // Handle close text box
                break;
            case MESSAGE_CHECK_CONNECTION:
                // Check network connection
                break;
            // ... handle other message types
            default:
                break;
        }
        osMsg = nativeGetMessageZennKuy();
    }
}
```

## Thread Safety

All operations are protected by `std::mutex` locks using `std::lock_guard`:
- **Atomic operations** - lock held only during queue operation
- **No deadlocks** - single lock per queue, lock_guard releases on scope exit
- **Exception safe** - std::lock_guard RAII ensures release even on exception

## Architecture

### RealGrowlauncherv5.57 Reference

The implementation mirrors the PowerKuy architecture from RealGrowlauncherv5.57:

```cpp
// From AppRenderer.java (Java/JNI layer)
int iNativeGetMessagePowerKuy = Main.PowerKuyRootRenderer.nativeGetMessagePowerKuy();
if (iNativeGetMessagePowerKuy != 0) {
    if (iNativeGetMessagePowerKuy == 1) {
        // Show keyboard
    } else if (iNativeGetMessagePowerKuy == 2) {
        // Hide keyboard
    }
}
```

Our C++ implementation provides the native backing for this pattern.

## Building

```bash
# Compile with C++11 or later
g++ -std=c++11 -pthread -c zennkuy.cpp -o zennkuy.o
ar rcs libzennkuy.a zennkuy.o

# Or as shared library
g++ -std=c++11 -pthread -fPIC -shared zennkuy.cpp -o libzennkuy.so
```

## Integration

### With JNI/Android

```cpp
// JNI wrapper
extern "C" {
    JNIEXPORT jint JNICALL 
    Java_launcher_powerkuy_growlauncher_MainActivity_nativeGetMessagePowerKuy(
        JNIEnv* env, jobject obj) {
        return nativeGetMessagePowerKuy();
    }
}
```

## Thread Model

- **Multi-producer, single consumer** - multiple threads can push messages safely
- **Lock-free reading** - returns immediately if empty
- **Priority-independent** - FIFO order for all messages
- **Unbounded queues** - will grow as needed (consider limits in production)

## Performance

- **Push**: O(1) amortized time complexity
- **Pop**: O(1) time complexity
- **Mutex contention**: Minimal - locks held only for queue operations
- **Memory**: Linear with queue size

## License

Part of Zennkuy launcher project.
