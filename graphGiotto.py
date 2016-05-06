import matplotlib.pyplot as plt

inputt = open('output.txt','r')
portdict = {}
for line in inputt:
	(port,val) = line.strip().split(',')
	try:
		portlist = portdict[port]
		portlist.append(int(val))
		portdict[port] = portlist
	except KeyError:
		portdict[port] = [int(val)]

# Drop zeroes from Weather Model Rainfall
rainfalls = portdict['rainfall']
rainfalls = [i for i in rainfalls if i != 0]
# Graph generated and sensed rainfall over time
plt.plot(rainfalls, label='Rainfall from Weather Model')
plt.plot(portdict['rainfallSensed'], label='Rainfall Accumulation on Sensor')
plt.legend(loc='upper right')
plt.title("Giotto Windshield Wiper Simulation\nRamping Weather Model")
plt.xlabel("Time")
plt.ylabel("Rainfall")

# Display the plot
plt.show()