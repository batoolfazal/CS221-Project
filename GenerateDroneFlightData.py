import time
import random
import math

START_LAT = 33.6844
START_LON = 73.0479
NUM_WAYPOINTS = 50

waypoints = []
current_lat = START_LAT
current_lon = START_LON
current_alt = 0.0

for i in range(NUM_WAYPOINTS):
    timestamp = time.time() + i * 0.1
    
    current_lat += random.uniform(-0.0001, 0.0001)
    current_lon += random.uniform(-0.0001, 0.0001)
    
    if i < NUM_WAYPOINTS * 0.2:
        current_alt = i * 2.5
    elif i < NUM_WAYPOINTS * 0.8:
        current_alt = 25.0 + random.uniform(-2, 2)
    else:
        descent = (i - NUM_WAYPOINTS * 0.8) / (NUM_WAYPOINTS * 0.2)
        current_alt = 25.0 * (1 - descent)
    
    roll = random.uniform(-15, 15)
    pitch = random.uniform(-15, 15)
    yaw = random.uniform(0, 360)
    vel_x = random.uniform(-5, 5)
    vel_y = random.uniform(-5, 5)
    vel_z = random.uniform(-2, 2)
    battery = 100 - (i / NUM_WAYPOINTS) * 80
    signal = random.uniform(70, 100)
    
    waypoints.append({
        'timestamp': timestamp,
        'latitude': current_lat,
        'longitude': current_lon,
        'altitude': current_alt,
        'roll': roll,
        'pitch': pitch,
        'yaw': yaw,
        'vel_x': vel_x,
        'vel_y': vel_y,
        'vel_z': vel_z,
        'battery': battery,
        'signal': signal
    })

with open("DroneFlightLog.csv", "w") as f:
    f.write("timestamp,latitude,longitude,altitude,roll,pitch,yaw,")
    f.write("vel_x,vel_y,vel_z,battery,signal\n")
    
    for wp in waypoints:
        f.write(f"{wp['timestamp']:.4f},")
        f.write(f"{wp['latitude']:.8f},")
        f.write(f"{wp['longitude']:.8f},")
        f.write(f"{wp['altitude']:.2f},")
        f.write(f"{wp['roll']:.2f},")
        f.write(f"{wp['pitch']:.2f},")
        f.write(f"{wp['yaw']:.2f},")
        f.write(f"{wp['vel_x']:.2f},")
        f.write(f"{wp['vel_y']:.2f},")
        f.write(f"{wp['vel_z']:.2f},")
        f.write(f"{wp['battery']:.1f},")
        f.write(f"{wp['signal']:.1f}\n")

print(f"✅ Generated {NUM_WAYPOINTS} waypoints → DroneFlightLog.csv")

