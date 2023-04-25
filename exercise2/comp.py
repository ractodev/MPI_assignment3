import numpy as np
from scipy.optimize import curve_fit

# Define the models for bandwidth and latency
def bandwidth(x, b):
    return b/x

def latency(x, a, b):
    return a + b/x

# Intra-node communication timings
intra_sizes = np.array([8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824])
intra_times = np.array([0.000001207, 0.000000336, 0.000000288, 0.000000283, 0.000000287, 0.000000338, 0.000000650, 0.000000692, 0.000001227, 0.000001600, 0.000000755, 0.000001201, 0.000001872, 0.000003659, 0.000006309, 0.000012160, 0.000030315, 0.000033491, 0.000057685, 0.000125674, 0.001075277, 0.002313561, 0.003774239, 0.006721470, 0.012858512, 0.024914216, 0.048973737, 0.089048648])

# Inter-node communication timings
inter_sizes = np.array([8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824])
inter_times = np.array([0.000001934, 0.000001954, 0.000002038, 0.000001947, 0.000002584, 0.000002791, 0.000002689, 0.000002804, 0.000002855, 0.000003032, 0.000003369, 0.000003719, 0.000006857, 0.000007825, 0.000010503, 0.000015975, 0.000026837, 0.000048733, 0.000092559, 0.000230860, 0.000364729, 0.000712871, 0.001415312, 0.002890288, 0.005702036, 0.011374468, 0.022734122, 0.057435230])

# Best fit for intra-node communication timings
popt_intra_bandwidth, _ = curve_fit(bandwidth, intra_sizes, intra_sizes/intra_times)
popt_intra_latency, _ = curve_fit(latency, intra_sizes, intra_times)

print("Intra-node communication:")
print("Bandwidth: {:.3f} MB/s".format(popt_intra_bandwidth[0]))
print("Latency: {:.3f} us".format(popt_intra_latency[0] * 1e6))

# Best fit for inter-node communication timings
popt_inter_bandwidth, _ = curve_fit(bandwidth, inter_sizes, inter_sizes/inter_times)
popt_inter_latency, _ = curve_fit(latency, inter_sizes, inter_times)

print("\nInter-node communication:")
print("Bandwidth: {:.3f} MB/s".format(popt_inter_bandwidth[0]))
print("Latency: {:.3f} us".format(popt_inter_latency[0] * 1e6))