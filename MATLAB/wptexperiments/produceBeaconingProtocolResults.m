close all
clear all

thresholds = [-70 -65 -60 -55 -50];

measurementsBeaconing = cell(length(thresholds), 1, 1);

for i=1:length(thresholds)
    currentThresholdFolder = abs(thresholds(i));
    folderName = ['beaconing/' int2str(currentThresholdFolder) 'dBm'];
    measurementsBeaconing{i} = importMeasurements(folderName);
    %generatePlots(measurements{i});
    [energyHarvestedLocal, efficiencyLocal, txPowerConsumedLocal] = generateEfficiencyAndEnergy(measurementsBeaconing{i});
    energyHarvestedBeaconing(:,i) = energyHarvestedLocal;
    efficiencyBeaconing(:,i) = efficiencyLocal;
    
    eventsBeaconing{i} = importEvents(folderName);
    [packetsSendLocal packetsReceivedLocal, timeRxOnLocal] = generateNumberOfPacketsSendAndReceived(eventsBeaconing{i});
    packetsSendBeaconing(:,i) = packetsSendLocal;
    packetsReceivedBeaconing(:,i) = packetsReceivedLocal;
    timeRxOnBeaconing(:,i) = timeRxOnLocal;
    
    [energyHarvestedMinusConsumption, energyConsumption] = powerConsumptionUpdate(timeRxOnLocal, packetsSendLocal, packetsReceivedLocal, 1000*energyHarvestedLocal);
    energyHarvestedBeaconing(:,i) = energyHarvestedMinusConsumption/1000;
    energyConsumptionBeaconing(:,i) = energyConsumption/1000;
    efficiencyBeaconing(:,i) = (energyHarvestedMinusConsumption/1000)./txPowerConsumedLocal;
    
end

measurementsProbing = cell(length(thresholds), 1, 1);

for i=1:length(thresholds)
    currentThresholdFolder = abs(thresholds(i));
    folderName = ['probing/' int2str(currentThresholdFolder) 'dBm'];
    measurementsProbing{i} = importMeasurements(folderName);  
    %generatePlots(measurements{i});
    [energyHarvestedLocal, efficiencyLocal, txPowerConsumedLocal] = generateEfficiencyAndEnergy(measurementsProbing{i});
    energyHarvestedProbing(:,i) = energyHarvestedLocal;
    efficiencyProbing(:,i) = efficiencyLocal;
    
    eventsProbing{i} = importEvents(folderName);
    [packetsSendLocal packetsReceivedLocal, timeRxOnLocal] = generateNumberOfPacketsSendAndReceived(eventsProbing{i});
    packetsSendProbing(:,i) = packetsSendLocal;
    packetsReceivedProbing(:,i) = packetsReceivedLocal;
    timeRxOnProbing(:,i) = timeRxOnLocal;
    
    [energyHarvestedMinusConsumption, energyConsumption] = powerConsumptionUpdate(timeRxOnLocal, packetsSendLocal, packetsReceivedLocal, 1000*energyHarvestedLocal);
    energyHarvestedProbing(:,i) = energyHarvestedMinusConsumption/1000;
    energyConsumptionProbing(:,i) = energyConsumption/1000;
    efficiencyProbing(:,i) = (energyHarvestedMinusConsumption/1000)./txPowerConsumedLocal;
end

folderName = 'meas101514_203607';
measurementsProbingBack = importMeasurements(folderName);
[energyHarvestedProbingBack, efficiencyProbingBack] = generateEfficiencyAndEnergy(measurementsProbingBack);
eventsProbingBack = importEvents(folderName);
[packetsSendProbingBack, packetsReceivedProbingBack] = generateNumberOfPacketsSendAndReceived(eventsProbingBack);

folderName = 'meas101514_221755';
measurementsBeaconingBack = importMeasurements(folderName);
[energyHarvestedBeaconingBack, efficiencyBeaconingBack] = generateEfficiencyAndEnergy(measurementsBeaconingBack);
eventsBeaconingBack = importEvents(folderName);
[packetsSendBeaconingBack, packetsReceivedBeaconingBack] = generateNumberOfPacketsSendAndReceived(eventsBeaconingBack);

energyHarvestedBack(:,1) = energyHarvestedBeaconing(:,1);
energyHarvestedBack(:,2) = energyHarvestedProbing(:,1);
energyHarvestedBack(:,3) = energyHarvestedBeaconingBack;
energyHarvestedBack(:,4) = energyHarvestedProbingBack;

efficiencyBack(:,1) = efficiencyBeaconing(:,1);
efficiencyBack(:,2) = efficiencyProbing(:,1);
efficiencyBack(:,3) = efficiencyBeaconingBack;
efficiencyBack(:,4) = efficiencyProbingBack;

packetsSendBack(:,1) = packetsSendBeaconing(:,1);
packetsSendBack(:,2) = packetsSendProbing(:,1);
packetsSendBack(:,3) = packetsSendBeaconingBack;
packetsSendBack(:,4) = packetsSendProbingBack;

packetsReceivedBack(:,1) = packetsReceivedBeaconing(:,1);
packetsReceivedBack(:,2) = packetsReceivedProbing(:,1);
packetsReceivedBack(:,3) = packetsReceivedBeaconingBack;
packetsReceivedBack(:,4) = packetsReceivedProbingBack;
% 
% figure;
% subplot(2,2,1)
% boxplot([energyHarvestedBeaconing*1000], 'labels', thresholds);
% ylim([200 2200])
% title('Energy');
% subplot(2,2,2)
% boxplot([efficiencyBeaconing*100], 'labels', thresholds);
% ylim([0 0.5])
% title('Efficiency');
% 
% 
% subplot(2,2,3)
% boxplot([energyHarvestedProbing*1000], 'labels', thresholds);
% ylim([200 2200])
% title('Energy');
% subplot(2,2,4)
% boxplot([efficiencyProbing*100], 'labels', thresholds);
% ylim([0 0.5])
% title('Efficiency');
% 
% figure;
% subplot(1,2,1)
% boxplot([energyHarvestedBack*1000], 'labels', {'-70bn', '-70pn', '-70bb', '-70pb'});
% ylim([200 2200])
% title('Energy');
% subplot(1,2,2)
% boxplot([efficiencyBack*100], 'labels', {'-70bn', '-70pn', '-70bb', '-70pb'});
% ylim([0 0.5])
% title('Efficiency');
% 
% 
% figure;
% subplot(2,2,1)
% boxplot(packetsSendBeaconing, 'labels', thresholds);
% %ylim([14 26])
% title('PacketsSend');
% subplot(2,2,2)
% boxplot(packetsReceivedBeaconing, 'labels', thresholds);
% %ylim([0 6])
% title('PacketsReceived');
% 
% 
% subplot(2,2,3)
% boxplot(packetsSendProbing, 'labels', thresholds);
% %ylim([14 26])
% title('PacketsSend');
% subplot(2,2,4)
% boxplot(packetsReceivedProbing, 'labels', thresholds);
% %ylim([0 6])
% title('PacketsReceived');

% 
% figure;
% subplot(1,2,1)
% boxplot(packetsSendBack, 'labels', {'-70bn', '-70pn', '-70bb', '-70pb'});
% %ylim([200 2200])
% title('PacketsSend');
% subplot(1,2,2)
% boxplot(packetsReceivedBack, 'labels', {'-70bn', '-70pn', '-70bb', '-70pb'});
% %ylim([0 0.5])
% title('PacketsReceived');
