
[numberOfExperiments, numberOfMeasurements] = size(energyHarvestedProbing);

for i=1:numberOfExperiments
    for j=1:numberOfMeasurements
        currentEnergy = 1000*energyHarvestedProbing(i,j);
        currentPacketsSend = packetsSendProbing(i,j);
        currentPacketsReceived = packetsReceivedProbing(i,j);
        currentExperimentTime = timeRxOnProbing(i,j);
        energyHarvestedMinusConsumption = powerConsumptionUpdate(currentExperimentTime, currentPacketsSend, currentPacketsReceived, currentEnergy);
        
        energyHarvestedProbing(i,j) = energyHarvestedMinusConsumption/1000;
    end
end


[numberOfExperiments, numberOfMeasurements] = size(energyHarvestedBeaconing);

for i=1:numberOfExperiments
    for j=1:numberOfMeasurements
        currentEnergy = 1000*energyHarvestedBeaconing(i,j);
        currentPacketsSend = packetsSendBeaconing(i,j);
        currentPacketsReceived = packetsReceivedBeaconing(i,j);
        currentExperimentTime = timeRxOnBeaconing(i,j);
        energyHarvestedMinusConsumption = powerConsumptionUpdate(currentExperimentTime, currentPacketsSend, currentPacketsReceived, currentEnergy);
        
        energyHarvestedBeaconing(i,j) = energyHarvestedMinusConsumption/1000;
    end
end