function [] = generatePlotsWithEvents(measurements, events)
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
        hold on;
        plot(measurements{i,j}.data(:,1)/1000, measurements{i,j}.data(:,2))
        
                
        xlim([0 xLimMax]);
        if (j > 1)
            ylim([0 1.2]);
        end
        
        timestampRxStart = zeros(1,11);
        timestampRxEnd = zeros(1,10);
        index = 0;
        % if this is Rx measurement
        if (j == 1)
            currentRxEvent = events{i,j};
            
            for k=1:length(currentRxEvent)
               
                if(strcmp(currentRxEvent{k,2}, 'buttonPress'))
                    index = index + 1;
                    timestampRxStart(index) = currentRxEvent{k,1};
                end
                
                if(strcmp(currentRxEvent{k,2}, 'buttonEnd'))
                    timestampRxEnd(index) = currentRxEvent{k,1};
                end
            end
            plot(timestampRxStart/1000, zeros(1, length(timestampRxStart)), 'r*')
            plot(timestampRxEnd/1000, zeros(1, length(timestampRxEnd)), 'g*')
        end
        
        
    end
end

end
