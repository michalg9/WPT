

MAX_MEASUREMENTS = 10;
MAX_TX = 4;
for i = 1:MAX_MEASUREMENTS
    for txNum = 1:MAX_TX
        filenameMeasurements = ['meas101514_190016/tx' int2str(txNum) '/EVENT' int2str(i) '.txt'];
        if (exist(filenameMeasurements, 'file') == 2)
            eventBeaconing{i, txNum} = importEventLog(filenameMeasurements);
        end
    end
end

[numberExperiments numberTx] = size(eventBeaconing);

index = 1;

figTx1 = figure(1);
hold on;
figTx2 = figure(2);
hold on;
figTx3 = figure(3);
hold on;
figTx4 = figure(4);
hold on;

figures = [figTx1 figTx2 figTx3 figTx4];
cc=hsv(7);

for i=1:numberExperiments
    for j=1:numberTx
        if (isempty(eventBeaconing{i,j}))
            continue;
        end
        
        currentEventBeaconing = eventBeaconing{i,j};
        for k=1:length(currentEventBeaconing)
           if (strcmp('packet', currentEventBeaconing{k,2}))
              rssiValues(index, 1) = currentEventBeaconing{k,1};
              rssiValues(index, 2) = currentEventBeaconing{k,7};
              index = index + 1;
           end
        end
        
        index = 1;
        figure(figures(j));
        plot(rssiValues(:,1)/1000, rssiValues(:,2), '*', 'color',cc(i,:));
    end
end