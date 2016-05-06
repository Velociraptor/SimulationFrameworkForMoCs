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

# Graph sensed rainfall over time
plt.plot(portdict['rainfallSensed'], label='Rainfall Accumulation on Sensor')
plt.legend(loc='upper right')
plt.title("PTIDES Windshield Wiper Simulation\nRandom Weather Model")
plt.xlabel("Time")
plt.ylabel("Rainfall")

# Display the plot
plt.show()