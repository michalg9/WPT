close all
clear all

folderName = 'meas081814_204637';
measurements = importMeasurements(folderName);
generatePlots(measurements);
[energyHarvested, efficiency] = generateEfficiencyAndEnergy(measurements);

thresholds = [-60 -55 -65 -70 -50];
[~,indSorted] = sort(thresholds);

for i=1:length(indSorted)
    energyHarvestedSorted1(i) = energyHarvested(indSorted(i));
    efficiencySorted1(i) = efficiency(indSorted(i));
end

folderName = 'meas081914_144947';
measurements = importMeasurements(folderName);
generatePlots(measurements);
[energyHarvestedSorted2, efficiencySorted2] = generateEfficiencyAndEnergy(measurements);


packetData

for i=1:length(indSorted)
    packetsSendCounterSorted(i) = 60*packetsSendCounter(indSorted(i))/lastTimestamp(indSorted(i));
end

packetsSendProbing = 60*packetsSendCounterProbing/lastTimestampProbing;
packetsReceivedProbing = 60*packetsReceivedCounterProbing/lastTimestampProbing;
figure;
subplot(4,1,1);
energyHarv = [energyHarvestedSorted1 energyHarvestedSorted2];
bar(energyHarv);
set(gca,'XTickLabel',{'-70dBm', '-65dBm', '-60dBm', '-55dBm', '-50dBm', 'probing'});
title('Energy harvested (J)');

subplot(4,1,2);
efficiency = [efficiencySorted1 efficiencySorted2];
bar(efficiency);
set(gca,'XTickLabel',{'-70dBm', '-65dBm', '-60dBm', '-55dBm', '-50dBm', 'probing'});
title('Efficiency');

subplot(4,1,3);
packetsSend = [packetsSendCounterSorted packetsSendProbing];
bar(packetsSend);
set(gca,'XTickLabel',{'-70dBm', '-65dBm', '-60dBm', '-55dBm', '-50dBm', 'probing'});
title('Packets send per minute');

subplot(4,1,4);
packetsReceived = [0 0 0 0 0 packetsReceivedProbing];
bar(packetsReceived);
set(gca,'XTickLabel',{'-70dBm', '-65dBm', '-60dBm', '-55dBm', '-50dBm', 'probing'});
title('Packets received per minute');