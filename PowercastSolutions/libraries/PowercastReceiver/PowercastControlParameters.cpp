//#include "PowercastControlParameters.h"
//
//ExperimentThresholds::ExperimentThresholds() {
//	commThreshold = TX4_RSSI_THRESHOLD;
//	powerThreshold = RX4_VOLTAGE_THRESHOLD;
//}
//
//void ExperimentThresholds::setCommThreshold(byte threshold) {
//	Serial.println(F("Communication threshold change"));
//	commThreshold = threshold;
//	printThresholds();
//}
//
//void ExperimentThresholds::setPowerThreshold(byte threshold) {
//	Serial.println(F("Power threshold change"));
//	powerThreshold = threshold;
//	printThresholds();
//}
//
//void ExperimentThresholds::printThresholds() {
//	Serial.print(F("Communication threshold: "));
//	Serial.println(commThreshold);
//
//	Serial.print(F("Power threshold: "));
//	Serial.println(powerThreshold);
//}
//
//ExperimentThresholds experimentThresholds;
