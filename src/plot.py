import matplotlib.pyplot as plt
import pandas as pd

# test data
# filename = 'data/test_P_30mph'
# filename = 'data/test_PI_PID_30mph'
filename = 'data/test_PI_PID_50mph'
# filename = 'data/test_PI_PID_60mph'
test = pd.read_csv(filename + '.csv')

# plot resut
time = test.iloc[:, 0]
speed = test.iloc[:, 1]
steer = test.iloc[:, 2]

# common
plt.figure(1)

plt.subplot(211)
plt.grid()
plt.ylabel("Speed [mile/h]")
plt.xlabel("Time [sec]")
plt.plot(time, speed)

plt.subplot(212)
plt.grid()
plt.ylabel("steering [rad]")
plt.xlabel("Time [sec]")
plt.plot(time, steer)

outname = 'fig/' + filename.split('/')[-1] + '.png'
plt.savefig(outname, dpi=300)
# plt.show()
