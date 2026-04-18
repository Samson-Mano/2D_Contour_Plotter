from circular_wave import CircularWave
import numpy as np
import struct
import os

wave1 = CircularWave(x0=-2.0, y0=0, freq_r=2.0, freq_t=0.1,phase = 0.0)
wave2 = CircularWave(x0=2.0, y0=0, freq_r=2.0, freq_t=0.1,phase = np.pi)


def wave_func(x, y, t):
  return wave1.evaluate(x, y, t) + wave2.evaluate(x, y, t)



# 1. Setup Simulation Parameters
time_points = np.arange(0.0, 1.0, 0.1) # 10 steps for testing
grid_res = 100
x = np.linspace(-10, 10, grid_res)
y = np.linspace(-10, 10, grid_res)
X, Y = np.meshgrid(x, y)
simtype = 0 # 0 = static, 1 = dynamic

# 2. Binary Export (Using your optimized plan)
# Format: 
# Header: GridX (int), GridY (int), TotalTime (int), Type (int) 0 = static, 1 = dynamic
# Per Frame: Time (float), MaxZ (float), MinZ (float), Z-Data (unsigned shorts)
with open("simulation_data.bin", "wb") as f:
  # Write Header
  f.write(struct.pack('iiii', grid_res, grid_res, len(time_points), simtype))

  for t in time_points:
    z_data = wave_func(X, Y, t)
    
    z_min, z_max = z_data.min(), z_data.max()
    # Prevent division by zero if z_min == z_max
    range_z = (z_max - z_min) if z_max != z_min else 1.0
    
    # Normalize and Pack to unsigned short (0 to 65535)
    z_packed = ((z_data - z_min) / range_z * 65535).astype(np.uint16)
    
    # Write Frame Metadata
    f.write(struct.pack('fff', t, z_max, z_min))
    # Write the entire Z-buffer for this frame
    f.write(z_packed.tobytes())

print(f"Binary file created. Size: {os.path.getsize('simulation_data.bin') / 1024:.2f} KB")

# 3. Simple Text Export (For manual verification of first few points)
with open("simulation_test.txt", "w") as f:
  f.write(f"Grid: {grid_res}x{grid_res}\n")
  for t in time_points[:2]: # Only first two steps to keep text file small
    z_data = wave_func(X, Y, t)
    f.write(f"Time: {t:.2f} | Max: {z_data.max():.4f} | Min: {z_data.min():.4f}\n")
    # Write first 5 z-values as a sample
    np.savetxt(f, z_data.flatten()[:5], fmt="%.4f")
    f.write("...\n")
