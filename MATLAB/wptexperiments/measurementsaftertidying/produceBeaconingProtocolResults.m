close all
clear all

thresholds = [-70 -65 -60 -55 -50];

measurements = cell(length(thresholds), 1, 1);

for i=1:length(thresholds)
    currentThresholdFolder = abs(thresholds(i));
    folderName = ['beaconingFixedNaming/' int2str(currentThresholdFolder) 'dBm'];
    measurements{i} = importMeasurements(folderName);
    %generatePlots(measurements{i});
    [energyHarvestedLocal, efficiencyLocal] = generateEfficiencyAndEnergy(measurements{i});
    
    energyHarvested(:,i) = energyHarvestedLocal;
    efficiency(:,i) = efficiencyLocal;
end


folderName = 'meas101314_222351';
measurementsProbing = importMeasurements(folderName);
[energyHarvestedProbing(:,1), efficiencyProbing(:,1)] = generateEfficiencyAndEnergy(measurementsProbing);
 
folderName = 'meas101414_170537';
measurementsProbing = importMeasurements(folderName);
[energyHarvestedProbing(:,2), efficiencyProbing(:,2)] = generateEfficiencyAndEnergy(measurementsProbing);
 
folderName = 'meas101414_180920';
measurementsProbing = importMeasurements(folderName);
[energyHarvestedProbing(:,3), efficiencyProbing(:,3)] = generateEfficiencyAndEnergy(measurementsProbing);


folderName = 'meas101414_194142fixedNames';
measurementsProbing = importMeasurements(folderName);
[energyHarvestedProbing(:,4), efficiencyProbing(:,4)] = generateEfficiencyAndEnergy(measurementsProbing);

folderName = 'meas101514_190016';
measurementsProbing = importMeasurements(folderName);
[energyHarvestedProbing(:,5), efficiencyProbing(:,5)] = generateEfficiencyAndEnergy(measurementsProbing);

folderName = 'meas101514_203607';
measurementsProbing = importMeasurements(folderName);
[energyHarvestedProbingBack, efficiencyProbingBack] = generateEfficiencyAndEnergy(measurementsProbing);

folderName = 'meas101514_221755';
measurementsProbing = importMeasurements(folderName);
[energyHarvestedBeaconingBack, efficiencyBeaconingBack] = generateEfficiencyAndEnergy(measurementsProbing);


energyHarvestedBack(:,1) = energyHarvested(:,1);
energyHarvestedBack(:,2) = energyHarvestedProbing(:,1);
energyHarvestedBack(:,3) = energyHarvestedBeaconingBack;
energyHarvestedBack(:,4) = energyHarvestedProbingBack;


efficiencyBack(:,1) = efficiency(:,1);
efficiencyBack(:,2) = efficiencyProbing(:,1);
efficiencyBack(:,3) = efficiencyBeaconingBack;
efficiencyBack(:,4) = efficiencyProbingBack;

figure;
subplot(2,2,1)
boxplot([energyHarvested*1000], 'labels', thresholds);
ylim([200 2200])
title('Energy');
subplot(2,2,2)
boxplot([efficiency*100], 'labels', thresholds);
ylim([0 0.5])
title('Efficiency');


subplot(2,2,3)
boxplot([energyHarvestedProbing*1000], 'labels', {'-70', '-65', '-60', '-55', '-50'});
ylim([200 2200])
title('Energy');
subplot(2,2,4)
boxplot([efficiencyProbing*100], 'labels', {'-70', '-65', '-60', '-55', '-50'});
ylim([0 0.5])
title('Efficiency');

figure;
subplot(1,2,1)
boxplot([energyHarvestedBack*1000], 'labels', {'-70bn', '-70pn', '-70bb', '-70pb'});
ylim([200 2200])
title('Energy');
subplot(1,2,2)
boxplot([efficiencyBack*100], 'labels', {'-70bn', '-70pn', '-70bb', '-70pb'});
ylim([0 0.5])
title('Efficiency');