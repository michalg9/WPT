function [ result ] = timeToStartCharging(N, K, timeNoSuccess, timeSuccess)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

result = 0;
for i = 1:(N-K+1)
    result = result + probChargedInRound(N, K, i)*((i-1)*timeNoSuccess + timeSuccess);
end

end

