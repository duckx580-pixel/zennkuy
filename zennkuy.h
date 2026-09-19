#ifndef ZENNKUY_H
#define ZENNKUY_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Zennkuy Message Queue System
 *
 * Provides thread-safe message queue implementation for native code
 * Based on PowerKuy architecture from RealGrowlauncherv5.57
 *
 * Two independent queues:
 * 1. Main OS message queue - for system messages (40+ types)
 * 2. PowerKuy soft keyboard queue - for UI keyboard events
 */

// Message type constants
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

// PowerKuy Soft Keyboard Messages
#define MESSAGE_SHOW_SOFT_KEYBOARD 1
#define MESSAGE_HIDE_SOFT_KEYBOARD 2

/**
 * Push a message to the main OS message queue
 * Thread-safe operation
 *
 * @param messageType The message type to queue
 */
void nativePushMessageZennKuy(int messageType);

/**
 * Get the next message from the message queue
 * Returns 0 (MESSAGE_NONE) if queue is empty
 * Otherwise returns the message type
 * Thread-safe operation
 *
 * @return Message type or 0 if no message
 */
int nativeGetMessageZennKuy();

/**
 * Push a PowerKuy soft keyboard message
 * Thread-safe operation
 *
 * @param messageType 1=show keyboard, 2=hide keyboard
 */
void nativePushMessagePowerKuy(int messageType);

/**
 * Get the next PowerKuy message from soft keyboard queue
 * Returns:
 *   0 = MESSAGE_NONE (no message)
 *   1 = MESSAGE_OPEN_TEXT_BOX (show soft keyboard)
 *   2 = MESSAGE_CLOSE_TEXT_BOX (hide soft keyboard)
 * Thread-safe operation
 *
 * @return Message type: 0, 1, or 2
 */
int nativeGetMessagePowerKuy();

/**
 * Clear all messages from both queues
 * Used during app shutdown or emergency situations
 * Thread-safe operation
 */
void nativeEmergencyMessageClear();

/**
 * Get the current size of the main message queue
 * For debugging purposes
 *
 * @return Queue size
 */
int nativeGetMessageQueueSize();

/**
 * Get the current size of the PowerKuy message queue
 * For debugging purposes
 *
 * @return Queue size
 */
int nativeGetPowerKuyMessageQueueSize();

/**
 * Initialize the message queue system
 * Call this at app startup
 */
void nativeInitializeMessageQueue();

/**
 * Shutdown the message queue system
 * Call this at app shutdown
 */
void nativeShutdownMessageQueue();

#ifdef __cplusplus
}
#endif

#endif // ZENNKUY_H
