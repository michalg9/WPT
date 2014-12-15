function [energyHarvestedMinusConsumption energyConsumption] = powerConsumptionUpdate(timeRxOn_s, packetsSend, packetsReceived, energyHarvested)

T_exp_s = timeRxOn_s;
duty_cycle = 1;

%XBee
U_V = 3.3;
% --- to change
I_Tx_mA = 35;
I_Rx_mA = 50;
I_active_mA = 50;
I_sleep_mA = 0.01;

Rb_bps = 9600;

% --- to change
S_pckt_b = 120;
N_pcktSend = packetsSend;
N_pcktRcv = packetsReceived;

T_pckt_s = S_pckt_b/Rb_bps;
T_pcktsSend_s = N_pcktSend*T_pckt_s;
T_pcktsRcv_s = N_pcktRcv*T_pckt_s;
T_active_s = T_exp_s*(1-duty_cycle);
T_sleep_s = T_exp_s*duty_cycle;

P_Tx_mW = U_V*I_Tx_mA;
P_Rx_mW = U_V*I_Rx_mA;
P_active_mW = U_V*I_active_mA;
P_sleep_mW = U_V*I_sleep_mA;

E_Rx_mJ = T_pcktsSend_s*P_Tx_mW;
E_Tx_mJ = T_pcktsRcv_s*P_Rx_mW;
E_active_mJ = T_active_s*P_active_mW;
E_sleep_mJ = T_sleep_s*P_sleep_mW;


E_Xbee_sum = E_Rx_mJ + E_Tx_mJ + E_active_mJ + E_sleep_mJ;

%Arduino
U_V = 3.3;
% --- to change
I_active_mA = 1.7;
I_sleep_mA = 0.0009;

T_packets_s = T_pcktsSend_s+T_pcktsRcv_s;
T_active_s = T_exp_s*(1-duty_cycle);
T_sleep_s = T_exp_s*duty_cycle;

P_packets_mW = U_V*I_active_mA;
P_active_mW = U_V*I_active_mA;
P_sleep_mW = U_V*I_sleep_mA;

E_packets_mJ = T_packets_s*P_active_mW;
E_active_mJ = T_active_s*P_active_mW;
E_sleep_mJ = T_sleep_s*P_sleep_mW;

E_ArduinoSum_mJ = E_packets_mJ + E_active_mJ + E_sleep_mJ;

energyConsumption = E_Xbee_sum' - E_ArduinoSum_mJ';
energyHarvestedMinusConsumption = energyHarvested - energyConsumption;


end

