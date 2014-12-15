function [] = generatePlots(measurements)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

[numberOfExperiments, numberOfMeasurements] = size(measurements);

for i = 1:numberOfExperiments
    
    if (isempty(measurements{i,1}))
        continue
    end
    
    xLimMax = 0;
    for j = 1:numberOfMeasurements
        if (measurements{i,j}.data(end,1) >= xLimMax)
            xLimMax = measurements{i,j}.data(end,1)/1000;
        end
    end
    
    figure;
    for j = 1:numberOfMeasurements
        subplot(numberOfMeasurements,1,j);
        plot(measurements{i,j}.data(:,1)/1000, measurements{i,j}.data(:,2))
        xlim([0 xLimMax]);
        if (j > 1)
            ylim([0 1.2]);
        end
    end
end

end
