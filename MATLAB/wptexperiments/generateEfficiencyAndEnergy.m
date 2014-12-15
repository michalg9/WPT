function [energyHarvested, efficiency, txPowerConsumed] = generateEfficiencyAndEnergy(measurements)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

[numberOfExperiments, numberOfMeasurements] = size(measurements);

resistance_Ohm = 1000;
powercastEnergyConsumption_W = 2.5;

index = 1;
for i=1:numberOfExperiments
     if (isempty(measurements{i,1}))
        continue
     end
    
    voltage_V = measurements{i,1}.data(:,2)/1000;
    power_W = (voltage_V.^2)/resistance_Ohm;
    time_s = measurements{i,1}.data(:,1)/1000;
    energy = trapz(time_s, power_W);

    for j = 2:numberOfMeasurements
        time_s = measurements{i,j}.data(:,1)/1000;
        isOnOff = measurements{i,j}.data(:,2);
        timeOn_s(j-1) = trapz(time_s, isOnOff);
    end
    energyHarvested(index) = energy;
    txPowerConsumed(index) = (powercastEnergyConsumption_W*sum(timeOn_s));
    efficiency(index) = energy/txPowerConsumed(index);
    index = index + 1;
end

end

