function [ result ] = probChargedInRound(N, K, i)
%PROBCHARGEDINROUND Summary of this function goes here
%   Detailed explanation goes here

% i - round Number

pi = zeros(1, i-1);
for j = 0:i-2
    pi(j+1) = (N-j-K)/(N-j);
end

result = prod(pi)*K/(N-(i-1));

end

