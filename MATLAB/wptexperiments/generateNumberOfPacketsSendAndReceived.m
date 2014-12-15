function [packetsSend, packetsReceived, timeRxOn_s] = generateNumberOfPacketsSendAndReceived(events)
%UNTITLED7 Summary of this function goes here
%   Detailed explanation goes here

[numberOfExperiments, numberOfMeasurements] = size(events);

codeActionSendPing = 0;
codeActionSendPowerProbe = 1;

codeEventPowerProbeRequestReceivedNotFromQueue = 1;
codeEventTimeoutWaitingForPower = 4;
codeEventVoltageAboveThreshold = 2;

for j=1:numberOfExperiments
    
    eventsFromRx = events{j,1};

    %beaconMessagesSentCounter = 0;
    %powerProbeMessagesSentCounter = 0;
    %powerProbeRequestMessagesReceivedCounter = 0;
    %eventTimeoutCounter = 0;
    %eventVoltageAboveCounter = 0;
    
    isTxOn = false;

    index = 0;
    
    timestampRxStart = zeros(1,11);
    timestampRxEnd = zeros(1,10);
            
    for i=1:length(eventsFromRx)
        if (strcmp(eventsFromRx{i,2}, 'buttonPress'))
            isTxOn = true;
            index = index + 1;
            
            beaconMessagesSentCounter(index) = 0;
            powerProbeMessagesSentCounter(index) = 0;
            powerProbeRequestMessagesReceivedCounter(index) = 0;
            eventTimeoutCounter(index) = 0;
            eventVoltageAboveCounter(index) = 0;
            
            timestampRxStart(index) = eventsFromRx{i,1};
        end

        if (strcmp(eventsFromRx{i,2}, 'buttonEnd'))
            isTxOn = false;
            
            timestampRxEnd(index) = eventsFromRx{i,1};
        end
        

        if (isTxOn)
            if (strcmp(eventsFromRx{i,2}, 'action'))
                if (eventsFromRx{i,3} == codeActionSendPing)
                    beaconMessagesSentCounter(index) = beaconMessagesSentCounter(index) + 1;
                elseif (eventsFromRx{i,3} == codeActionSendPowerProbe)
                    powerProbeMessagesSentCounter(index) = powerProbeMessagesSentCounter(index) + 1;
                end
            end

            if (strcmp(eventsFromRx{i,2}, 'event'))
                if (eventsFromRx{i,3} == codeEventPowerProbeRequestReceivedNotFromQueue)
                    powerProbeRequestMessagesReceivedCounter(index) = powerProbeRequestMessagesReceivedCounter(index) + 1;
                end
                
                if (eventsFromRx{i,3} == codeEventTimeoutWaitingForPower)
                    eventTimeoutCounter(index) = eventTimeoutCounter(index) + 1;
                end
                
                if (eventsFromRx{i,3} == codeEventVoltageAboveThreshold)
                    eventVoltageAboveCounter(index) = eventVoltageAboveCounter(index) + 1;
                end
            end          
        end 
    end
    
    packetsSend(j, :) = beaconMessagesSentCounter + powerProbeMessagesSentCounter;
    packetsReceived(j, :) = powerProbeRequestMessagesReceivedCounter;
    eventsTimeout(j, :) = eventTimeoutCounter;
    eventsVoltageAbove(j, :) = eventVoltageAboveCounter;
    
    timestampRxStart = timestampRxStart(1:end-1);
    
    timeLengthsRxOn = timestampRxEnd - timestampRxStart;
    timeRxOn_s(j, :) = sum(timeLengthsRxOn)/1000;
    
end

%timeRxOn_s = sum(timeLengthsRxOn)/1000
%per minute
%packetsSend = sum(packetsSend, 2)/timeRxOn_s*60;
%packetsReceived = sum(packetsReceived, 2)/timeRxOn_s*60;

%total
packetsSend = sum(packetsSend, 2);
packetsReceived = sum(packetsReceived, 2);

end

