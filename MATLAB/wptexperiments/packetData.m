

MAX_MEASUREMENTS = 5;

for i = 1:MAX_MEASUREMENTS
        filenameMeasurements = ['meas081814_204637/rx1/EVENT' int2str(i) '.txt'];
        if (exist(filenameMeasurements, 'file') == 2)
            eventBeaconing{i} = importBeaconingEvent(filenameMeasurements);
        end
end
isInNetwork = false;
for i = 1:length(eventBeaconing)
   eventBeaconingCurrent = eventBeaconing{i};
   packetsSendCounter(i) = 0;
   for j = 1:length(eventBeaconingCurrent)
      if (strcmp('buttonPress', eventBeaconingCurrent{j,2}))
         isInNetwork = true;
         lastTimestamp(i) = eventBeaconingCurrent{j,1}/1000;
      elseif (strcmp('buttonEnd', eventBeaconingCurrent{j,2}))
          isInNetwork = false;
      end
      
      if (isInNetwork && strcmp('action', eventBeaconingCurrent{j,2}) && eventBeaconingCurrent{j,3} == 0)
         packetsSendCounter(i) = packetsSendCounter(i) + 1; 
      end
   end
end


eventProbing = importProbingEvent('meas081914_144947/rx1/EVENT1.txt');

isInNetwork = false;

packetsSendCounterProbing = 0;
packetsReceivedCounterProbing = 0;
for i = 1:length(eventProbing)
      if (strcmp('buttonPress', eventProbing{i,2}))
         isInNetwork = true; 
         lastTimestampProbing = str2num(eventProbing{i,1})/1000;
      elseif (strcmp('buttonEnd', eventProbing{i,2}))
          isInNetwork = false;
      end
      
      if (isInNetwork && strcmp('action', eventProbing{i,2}) && (eventProbing{i,3} == 0 || eventProbing{i,3} == 1))
         packetsSendCounterProbing = packetsSendCounterProbing + 1; 
      end
      
       if (isInNetwork && strcmp('event', eventProbing{i,2}) && (eventProbing{i,3} == 2 || eventProbing{i,3} == 3 || eventProbing{i,3} == 4))
         packetsReceivedCounterProbing = packetsReceivedCounterProbing + 1; 
      end
end
