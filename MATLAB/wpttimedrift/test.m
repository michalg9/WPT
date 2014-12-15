clear all;
close all;

rxEvents{1} = importBeaconingEvent('meas1_noSynch_ar03\rx\EVENT1.txt');
txEvents{1} = importBeaconingEvent('meas1_noSynch_ar03\tx\EVENT1.txt');
rxEvents{2} = importBeaconingEvent('meas2_noSynch_ar03\rx\EVENT3.txt');
txEvents{2} = importBeaconingEvent('meas2_noSynch_ar03\tx\EVENT3.txt');
rxEvents{4} = importBeaconingEvent('meas1_30s_ar03\rx\EVENT2.txt');
txEvents{4} = importBeaconingEvent('meas1_30s_ar03\tx\EVENT2.txt');
rxEvents{5} = importBeaconingEvent('meas2_30s_ar03\rx\EVENT1.txt');
txEvents{5} = importBeaconingEvent('meas2_30s_ar03\tx\EVENT1.txt');
rxEvents{6} = importBeaconingEvent('meas3_30s_ar03\rx\EVENT3.txt');
txEvents{6} = importBeaconingEvent('meas3_30s_ar03\tx\EVENT3.txt');
rxEvents{7} = importBeaconingEvent('meas1_30s_ar01\rx\EVENT1.txt');
txEvents{7} = importBeaconingEvent('meas1_30s_ar01\tx\EVENT1.txt');
rxEvents{8} = importBeaconingEvent('meas1_30s_ar04\rx\EVENT3.txt');
txEvents{8} = importBeaconingEvent('meas1_30s_ar04\tx\EVENT3.txt');
rxEvents{9} = importBeaconingEvent('meas1_noSynch_ar04\rx\EVENT5.txt');
txEvents{9} = importBeaconingEvent('meas1_noSynch_ar04\tx\EVENT5.txt');
rxEvents{10} = importBeaconingEvent('meas1_noSynch_ar01\rx\EVENT1.txt');
txEvents{10} = importBeaconingEvent('meas1_noSynch_ar01\tx\EVENT1.txt');
rxEvents{11} = importBeaconingEvent('meas1_2s_ar03\rx\EVENT1.txt');
txEvents{11} = importBeaconingEvent('meas1_2s_ar03\tx\EVENT1.txt');
rxEvents{12} = importBeaconingEvent('meas1_2s_ar04\rx\EVENT1.txt');
txEvents{12} = importBeaconingEvent('meas1_2s_ar04\tx\EVENT1.txt');
rxEvents{13} = importBeaconingEvent('meas1_2s_ar01\rx\EVENT1.txt');
txEvents{13} = importBeaconingEvent('meas1_2s_ar01\tx\EVENT1.txt');

clear rxPingActionTimestamps;
clear txPingActionTimestamps;
clear differences;
currentRxEvents = rxEvents{4};
currentTxEvents = txEvents{4};
index = 1;
for i=1:length(currentRxEvents)
    if (strcmp(currentRxEvents(i, 2), 'action'))
        timestampCell = currentRxEvents(i, 1);
        rxPingActionTimestamps(index) = timestampCell{1};
        index = index + 1;
    end
end

index = 1;
for i=1:length(currentTxEvents)
    if (strcmp(currentTxEvents(i, 2), 'packet'))
        timestampCell = currentTxEvents(i, 1);
        txPingActionTimestamps(index) = timestampCell{1};
        index = index + 1;
    end
end

differences = rxPingActionTimestamps - txPingActionTimestamps;
figure;
hold on;
plot(rxPingActionTimestamps/1000, differences/1000, 'b')

clear rxPingActionTimestamps;
clear txPingActionTimestamps;
clear differences;
currentRxEvents = rxEvents{11};
currentTxEvents = txEvents{11};
index = 1;
for i=1:length(currentRxEvents)
    if (strcmp(currentRxEvents(i, 2), 'action'))
        timestampCell = currentRxEvents(i, 1);
        rxPingActionTimestamps(index) = timestampCell{1};
        index = index + 1;
    end
end

index = 1;
for i=1:length(currentTxEvents)
    if (strcmp(currentTxEvents(i, 2), 'packet'))
        timestampCell = currentTxEvents(i, 1);
        txPingActionTimestamps(index) = timestampCell{1};
        index = index + 1;
    end
end

differences = rxPingActionTimestamps - txPingActionTimestamps;
plot(rxPingActionTimestamps/1000, differences/1000, 'r')
% 
% 
% clear rxPingActionTimestamps;
% clear txPingActionTimestamps;
% clear differences;
% currentRxEvents = rxEvents{7};
% currentTxEvents = txEvents{7};
% index = 1;
% for i=1:length(currentRxEvents)
%     if (strcmp(currentRxEvents(i, 2), 'action'))
%         timestampCell = currentRxEvents(i, 1);
%         rxPingActionTimestamps(index) = timestampCell{1};
%         index = index + 1;
%     end
% end
% 
% index = 1;
% for i=1:length(currentTxEvents)
%     if (strcmp(currentTxEvents(i, 2), 'packet'))
%         timestampCell = currentTxEvents(i, 1);
%         txPingActionTimestamps(index) = timestampCell{1};
%         index = index + 1;
%     end
% end
% 
% differences = rxPingActionTimestamps - txPingActionTimestamps;
% plot(rxPingActionTimestamps/1000, differences/1000, 'g')

% 
% clear rxPingActionTimestamps;
% clear txPingActionTimestamps;
% clear differences;
% currentRxEvents = rxEvents{2};
% currentTxEvents = txEvents{2};
% index = 1;
% for i=1:length(currentRxEvents)
%     if (strcmp(currentRxEvents(i, 2), 'action'))
%         timestampCell = currentRxEvents(i, 1);
%         rxPingActionTimestamps(index) = timestampCell{1};
%         index = index + 1;
%     end
% end
% 
% index = 1;
% for i=1:length(currentTxEvents)
%     if (strcmp(currentTxEvents(i, 2), 'packet'))
%         timestampCell = currentTxEvents(i, 1);
%         txPingActionTimestamps(index) = timestampCell{1};
%         index = index + 1;
%     end
% end
% 
% differences = rxPingActionTimestamps(2:end) - txPingActionTimestamps(2:end);
% 
% plot(rxPingActionTimestamps(2:end)/1000, differences/1000)
% 
% 
% 
% clear rxPingActionTimestamps;
% clear txPingActionTimestamps;
% clear differences;
% currentRxEvents = rxEvents{3};
% currentTxEvents = txEvents{3};
% index = 1;
% for i=1:length(currentRxEvents)
%     if (strcmp(currentRxEvents(i, 2), 'action'))
%         timestampCell = currentRxEvents(i, 1);
%         rxPingActionTimestamps(index) = timestampCell{1};
%         index = index + 1;
%     end
% end
% 
% index = 1;
% for i=1:length(currentTxEvents)
%     if (strcmp(currentTxEvents(i, 2), 'packet'))
%         timestampCell = currentTxEvents(i, 1);
%         txPingActionTimestamps(index) = timestampCell{1};
%         index = index + 1;
%     end
% end
% 
% differences = rxPingActionTimestamps - txPingActionTimestamps;

%plot(rxPingActionTimestamps/1000, differences/1000)
