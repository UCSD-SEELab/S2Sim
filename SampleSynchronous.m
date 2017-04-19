function SampleSynchronous( clientData, objectName, serverAddress )
% Input is clientData and objectName
if nargin < 2
clientData = 1:192;
objectName = 'UCSD1';
serverAddress = 'seelabc.ucsd.edu';
elseif nargin ~= 3
    serverAddress = 'seelabc.ucsd.edu';
end

pHistory = zeros(length(clientData));
cHistory = zeros(length(clientData));

% Open up the TCP connection
tcpIp = tcpip(serverAddress,26999, 'NetworkRole', 'client');
tcpIp.Timeout = Inf;
fopen(tcpIp);

% Create Message Structure
startOfMessage = [hex2dec('12') hex2dec('34') hex2dec('56') hex2dec('78')];
senderId = [255, 255];
receiverId = [0,0];
sequenceNumber = [0,0,0,0];
syncConnectionRequestType = [0,1,0,4];
clientDataType = [0,3,0,5];

padding = zeros(1,4-mod(length(objectName),4));

syncConnectionData = [objectName, padding];
syncConnectionDataSizeHex = dec2hex(length(syncConnectionData),8);
syncConnectionDataSize = [hex2dec(syncConnectionDataSizeHex(1:2)), hex2dec(syncConnectionDataSizeHex(3:4)), hex2dec(syncConnectionDataSizeHex(5:6)), hex2dec(syncConnectionDataSizeHex(7:8))];
endOfMessage = [hex2dec('FE') hex2dec('DC') hex2dec('BA') hex2dec('98')];

% Create Connection Request and Send it
syncConnectionRequest = [startOfMessage,senderId,receiverId,sequenceNumber,syncConnectionRequestType,syncConnectionDataSize,syncConnectionData,endOfMessage];

pause(3);
%fwrite(tcpIp,syncConnectionRequest);

%for i=1:100
    fwrite(tcpIp,syncConnectionRequest);
  %  pause(3);
%end

% Read the response and get your ID
syncConnectionResponse = fread(tcpIp,36)
senderId = syncConnectionResponse(7:8)';
disp(['You are ID: ' num2str(senderId(1)*2^8 + senderId(2))]);

% Check the Request Result for approval
requestResult = syncConnectionResponse(21:24);
requestResult = requestResult(1) * 2^24 + requestResult(2) * 2^16 + requestResult(3) * 2^8 + requestResult(4);
if ( requestResult == 0 )
    disp('Accepted')
else
    disp('Your name is Not Found');
    return;
end

% System time for information
systemTime = syncConnectionResponse(25:28);
systemTime = systemTime(1)*2^24+systemTime(2)*2^16+systemTime(3)*2^8+systemTime(4);
disp(['Current System Time: ' num2str(systemTime)]);

% Number of Clients for information
numberOfClients = syncConnectionResponse(29:30);
numberOfClients = numberOfClients(1)*2^8+numberOfClients(2);
disp(['Number Of Clients: ' num2str(numberOfClients)]);

% System Mode for information
systemMode = syncConnectionResponse(31:32);
systemMode = systemMode(1)*2^8+systemMode(2);
disp(['System Mode: ' num2str(systemMode)]);

% Simulation Time Step for information
simulationTimeStep = syncConnectionResponse(33:36);
simulationTimeStep = simulationTimeStep(1)*2^24+simulationTimeStep(2)*2^16+simulationTimeStep(3)*2^8+simulationTimeStep(4);
disp(['Simulation Time Step: ' num2str(simulationTimeStep)]);

for i=1:length(clientData)
    % Receive the price signal and parse.
    setPriceMsg = fread(tcpIp,36);
    disp('Set Price Received');
    interval = setPriceMsg(25:28);
    numPricePoints = setPriceMsg(29:32);
    price = setPriceMsg(33:36);
    interval = interval(1)*2^24+interval(2)*2^16+interval(3)*2^8+interval(4);
    numPricePoints = numPricePoints(1)*2^24+numPricePoints(2)*2^16+numPricePoints(3)*2^8+numPricePoints(4);
    price = price(1)*2^24+price(2)*2^16+price(3)*2^8+price(4);

    disp(['Price Interval: ' num2str(interval) ' Num of Price Received: ' num2str(numPricePoints) ' Price: ' num2str(price)]);

    % Store the signals and send the next consumption
    consumption = clientData(i);
    pHistory(i) = price;
    cHistory(i) = consumption;
    consumptionHex = dec2hex(uint32(consumption),8);
    consumptionData = [hex2dec(consumptionHex(1:2)), hex2dec(consumptionHex(3:4)), hex2dec(consumptionHex(5:6)), hex2dec(consumptionHex(7:8))];
    clientDataMsg = [startOfMessage,senderId,receiverId,sequenceNumber,clientDataType,syncConnectionDataSize,consumptionData,endOfMessage];
    fwrite(tcpIp,clientDataMsg);
end
fclose(tcpIp);
end