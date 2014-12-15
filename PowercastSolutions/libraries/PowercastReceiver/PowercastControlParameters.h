#ifndef PowercastControlParameters_h
#define PowercastControlParameters_h

#include "Arduino.h"

// from PowercastRxControl3Layer.h
#define DELAY_PING 4000U
#define DELAY_REMOVE_LAST_PROBE_SENDER 20000U
#define DELAY_WAITING_FOR_SECOND_PROBE 6000U

#define VOLTAGE_THRESHOLD 600

// from PowercastTxControl2Layer.h
//#define TIMEOUT_PING 11000U
#define TIMEOUT_PING DELAY_PING*2

#define TIMEOUT_ON_PROBE DELAY_PING
#define FIRST_CHECK_AFTER 2000U

//#define TIMEOUT_PROBE_SENT 10000U
#define TIMEOUT_PROBE_SENT TIMEOUT_PING
#define RSSI_THRESHOLD 70

// for v4
// test
#define RX4_DELAY_PING 4000U
#define RX4_DELAY_REMOVE_LAST_PROBE_SENDER 30000U
#define RX4_DELAY_WAITING_FOR_POWER 4000U
#define RX4_DELAY_POWER_PROBING RX4_DELAY_PING
#define RX4_VOLTAGE_THRESHOLD 500

#define TX4_TIMEOUT_PING RX4_DELAY_PING*2
#define TX4_TIMEOUT_POWER_PROBE_RESPONSE 4000U
#define TX4_TURN_OFF_IF_FIRST_PWR_PROBE_NOT_RECEIVED_AFTER 2000U
#define TX4_TIMEOUT_TX_PROBE_RECEIVE RX4_DELAY_POWER_PROBING*2
#define TX4_RSSI_THRESHOLD 70
#define TX4_RANDOM_WAIT_TIME_MAX 500

//
//class ExperimentThresholds {
//public:
//	byte commThreshold;
//	int powerThreshold;
//
//	ExperimentThresholds();
//	
//	void setCommThreshold(byte threshold);
//
//	void setPowerThreshold(byte threshold);
//
//	void printThresholds();
//};

#endif
/*
// from PowercastRxControl3Layer.h
#define DELAY_PING 6000U
#define DELAY_REMOVE_LAST_PROBE_SENDER 5000U
#define DELAY_WAITING_FOR_SECOND_PROBE 4000U

#define VOLTAGE_THRESHOLD 1000

// from PowercastTxControl2Layer.h
//#define TIMEOUT_PING 11000U
#define TIMEOUT_PING DELAY_PING + 1000U

#define TIMEOUT_ON_PROBE TIMEOUT_PING
#define FIRST_CHECK_AFTER 2000U

//#define TIMEOUT_PROBE_SENT 10000U
#define TIMEOUT_PROBE_SENT TIMEOUT_PING

#define RSSI_THRESHOLD 70
*/