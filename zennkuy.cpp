#include <queue>
#include <mutex>
#include <cstring>

// Message types - matches PowerKuy implementation
#define MESSAGE_NONE 0
#define MESSAGE_OPEN_TEXT_BOX 1
#define MESSAGE_CLOSE_TEXT_BOX 2
#define MESSAGE_CHECK_CONNECTION 3
#define MESSAGE_SET_FPS_LIMIT 4
#define MESSAGE_SET_ACCELEROMETER_UPDATE_HZ 5
#define MESSAGE_FINISH_APP 6
#define MESSAGE_SET_VIDEO_MODE 7
#define MESSAGE_TAPJOY_GET_FEATURED_APP 8
#define MESSAGE_TAPJOY_GET_AD 9
#define MESSAGE_TAPJOY_GET_MOVIE 10
#define MESSAGE_TAPJOY_SHOW_FEATURED_APP 11
#define MESSAGE_TAPJOY_SHOW_AD 12
#define MESSAGE_TAPJOY_SHOW_MOVIE_AD 13
#define MESSAGE_IAP_PURCHASE 14
#define MESSAGE_IAP_GET_PURCHASED_LIST 15
#define MESSAGE_TAPJOY_GET_TAP_POINTS 16
#define MESSAGE_TAPJOY_SPEND_TAP_POINTS 17
#define MESSAGE_TAPJOY_AWARD_TAP_POINTS 18
#define MESSAGE_TAPJOY_SHOW_OFFERS 19
#define MESSAGE_HOOKED_SHOW_RATE_DIALOG 20
#define MESSAGE_ALLOW_SCREEN_DIMMING 21
#define MESSAGE_REQUEST_AD_SIZE 22
#define MESSAGE_CHARTBOOST_CACHE_INTERSTITIAL 23
#define MESSAGE_CHARTBOOST_SHOW_INTERSTITIAL 24
#define MESSAGE_CHARTBOOST_CACHE_MORE_APPS 25
#define MESSAGE_CHARTBOOST_SHOW_MORE_APPS 26
#define MESSAGE_CHARTBOOST_SETUP 27
#define MESSAGE_CHARTBOOST_NOTIFY_INSTALL 28
#define MESSAGE_CHARTBOOST_RESERVED1 29
#define MESSAGE_CHARTBOOST_RESERVED2 30
#define MESSAGE_FLURRY_SETUP 31
#define MESSAGE_FLURRY_ON_PAGE_VIEW 32
#define MESSAGE_FLURRY_LOG_EVENT 33
#define MESSAGE_SUSPEND_TO_HOME_SCREEN 34
#define MESSAGE_TAPJOY_INIT_MAIN 35
#define MESSAGE_TAPJOY_INIT_PAID_APP_WITH_ACTIONID 36
#define MESSAGE_TAPJOY_SET_USERID 37
#define MESSAGE_IAP_CONSUME_ITEM 38
#define MESSAGE_IAP_ITEM_DETAILS 39
#define MESSAGE_APPSFLYER_LOG_PURCHASE 40
#define MESSAGE_OPEN_TEXTBOX_SECRET 41

// Global message queue with thread-safe access
static std::queue<int> g_messageQueue;
static std::mutex g_messageQueueMutex;

// PowerKuy soft keyboard message queue (1=show, 2=hide)
static std::queue<int> g_powerKuyMessageQueue;
static std::mutex g_powerKuyMessageQueueMutex;

/**
 * Push a message to the main OS message queue
 * Thread-safe operation
 */
void nativePushMessageZennKuy(int messageType) {
    std::lock_guard<std::mutex> lock(g_messageQueueMutex);
    g_messageQueue.push(messageType);
}

/**
 * Get the next message from the message queue
 * Returns 0 if queue is empty (MESSAGE_NONE)
 * Otherwise returns the message type
 * Thread-safe operation
 */
int nativeGetMessageZennKuy() {
    std::lock_guard<std::mutex> lock(g_messageQueueMutex);

    if (g_messageQueue.empty()) {
        return 0; // MESSAGE_NONE
    }

    int message = g_messageQueue.front();
    g_messageQueue.pop();
    return message;
}

/**
 * Push a PowerKuy soft keyboard message
 * 1 = show soft keyboard
 * 2 = hide soft keyboard
 */
void nativePushMessagePowerKuy(int messageType) {
    std::lock_guard<std::mutex> lock(g_powerKuyMessageQueueMutex);
    g_powerKuyMessageQueue.push(messageType);
}

/**
 * Get the next PowerKuy message from the soft keyboard message queue
 * Returns:
 *   0 = no message / MESSAGE_NONE
 *   1 = show soft keyboard / MESSAGE_OPEN_TEXT_BOX
 *   2 = hide soft keyboard / MESSAGE_CLOSE_TEXT_BOX
 * Thread-safe operation
 */
int nativeGetMessagePowerKuy() {
    std::lock_guard<std::mutex> lock(g_powerKuyMessageQueueMutex);

    if (g_powerKuyMessageQueue.empty()) {
        return 0; // MESSAGE_NONE - no message
    }

    int message = g_powerKuyMessageQueue.front();
    g_powerKuyMessageQueue.pop();

    // Only return 1 or 2, anything else returns 0
    if (message == 1 || message == 2) {
        return message;
    }

    return 0;
}

/**
 * Clear all messages from both queues
 * Used during app shutdown or emergency situations
 */
void nativeEmergencyMessageClear() {
    {
        std::lock_guard<std::mutex> lock(g_messageQueueMutex);
        while (!g_messageQueue.empty()) {
            g_messageQueue.pop();
        }
    }

    {
        std::lock_guard<std::mutex> lock(g_powerKuyMessageQueueMutex);
        while (!g_powerKuyMessageQueue.empty()) {
            g_powerKuyMessageQueue.pop();
        }
    }
}

/**
 * Get the size of the message queue (for debugging)
 */
int nativeGetMessageQueueSize() {
    std::lock_guard<std::mutex> lock(g_messageQueueMutex);
    return g_messageQueue.size();
}

/**
 * Get the size of the PowerKuy message queue (for debugging)
 */
int nativeGetPowerKuyMessageQueueSize() {
    std::lock_guard<std::mutex> lock(g_powerKuyMessageQueueMutex);
    return g_powerKuyMessageQueue.size();
}

/**
 * Initialize the message queue system
 * Call this at app startup
 */
void nativeInitializeMessageQueue() {
    // Clear all existing messages
    nativeEmergencyMessageClear();
}

/**
 * Shutdown the message queue system
 * Call this at app shutdown
 */
void nativeShutdownMessageQueue() {
    nativeEmergencyMessageClear();
}

// Example usage function showing the message queue pattern
// This mirrors the PowerKuy implementation from AppRenderer.java
int exampleMessageQueueUsage() {
    // Get next PowerKuy message (soft keyboard)
    int powerKuyMsg = nativeGetMessagePowerKuy();
    if (powerKuyMsg != 0) {
        if (powerKuyMsg == 1) {
            // Show soft keyboard
            // ((InputMethodManager) app.getSystemService("input_method"))
            //     .toggleSoftInput(2, 0);
        } else if (powerKuyMsg == 2) {
            // Hide soft keyboard
            // ((InputMethodManager) app.getSystemService("input_method"))
            //     .hideSoftInputFromWindow(window.getWindowToken(), 0);
        }
    }

    // Get and handle OS messages from the main queue
    int osMsg = nativeGetMessageZennKuy();
    if (osMsg != 0) {
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
            case MESSAGE_SET_FPS_LIMIT:
                // Set FPS limit
                break;
            case MESSAGE_FINISH_APP:
                // Finish/close app
                break;
            // ... handle other messages
            default:
                // Unhandled message
                break;
        }
    }

    return osMsg;
}
