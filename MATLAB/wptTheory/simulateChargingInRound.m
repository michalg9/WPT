function [ probabilityOfChargingInRound ] = simulateChargingInRound(N, K, count)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

chargedInRoundCounter = zeros(1, N-K+1);

for j = 1:count

    txInCommRange = 1:N;
    txInChargingRange = 1:K;
    
    for i = 1:(N-K+1)
        % random choice of Tx
        currentTx = txInCommRange(randi(length(txInCommRange)));
        if (any(txInChargingRange == currentTx))
            chargedInRoundCounter(i) = chargedInRoundCounter(i) + 1;
            break;
        else
           txInCommRange = txInCommRange(txInCommRange ~= currentTx);
        end
    end
end

probabilityOfChargingInRound = chargedInRoundCounter/count;

end

