close all
clear all

thresholds = [-70 -65 -60 -55 -50];

measurements = cell(length(thresholds), 1, 1);

for i=1:length(thresholds)
    currentThresholdFolder = abs(thresholds(i));
    folderName = 'meas101314_222351';
    measurements{i} = importMeasurements(folderName);
    %generatePlots(measurements{i});
    [energyHarvestedLocal, efficiencyLocal] = generateEfficiencyAndEnergy(measurements{i});
    
    energyHarvested(:,i) = energyHarvestedLocal;
    efficiency(:,i) = efficiencyLocal;
end
% 
% figure;
% boxplot(energyHarvested*1000, 'labels', thresholds);
% 
% figure;
% boxplot(efficiency*100, 'labels', thresholds);