"""
Dynamic Hazard Generator for Drone Flight Simulation
"""

import random
import time

def generate_hazards(map_width, map_height, num_hazards, output_file):
    """Generate random hazards"""
    
    hazard_types = {
        "BIRD": {"duration": (5, 20), "severity": (2, 4), "desc": "Bird flock"},
        "DRONE": {"duration": (10, 30), "severity": (3, 5), "desc": "Unauthorized drone"},
        "WEATHER": {"duration": (30, 60), "severity": (1, 3), "desc": "Weather system"},
        "DEBRIS": {"duration": (15, 45), "severity": (3, 5), "desc": "Falling debris"},
        "AIRCRAFT": {"duration": (20, 40), "severity": (5, 5), "desc": "Aircraft nearby"}
    }
    
    hazards = []
    
    print(f"🎲 Generating {num_hazards} hazards for {map_width}x{map_height} map")
    print()
    
    for i in range(num_hazards):
        x = random.randint(0, map_width - 1)
        y = random.randint(0, map_height - 1)
        
        hazard_type = random.choice(list(hazard_types.keys()))
        props = hazard_types[hazard_type]
        
        duration = random.randint(*props["duration"])
        severity = random.randint(*props["severity"])
        timestamp = int(time.time()) + random.randint(0, 300)
        
        description = f"{props['desc']} at sector {i+1}"
        
        hazards.append({
            'x': x,
            'y': y,
            'type': hazard_type,
            'description': description,
            'duration': duration,
            'severity': severity,
            'timestamp': timestamp
        })
        
        print(f"Hazard {i+1}: ({x},{y}) - {hazard_type} [Sev:{severity}/5, {duration}s]")
    
    # Sort by severity (highest first)
    hazards.sort(key=lambda h: h['severity'], reverse=True)
    
    # Save to file
    with open(output_file, 'w') as f:
        f.write(f"# Hazard Detection Data\n")
        f.write(f"# Map: {map_width}x{map_height}\n")
        f.write(f"# Total: {num_hazards} hazards\n")
        f.write(f"# Format: X Y TYPE DESCRIPTION DURATION SEVERITY TIMESTAMP\n\n")
        
        for h in hazards:
            f.write(f"{h['x']} {h['y']} {h['type']} \"{h['description']}\" ")
            f.write(f"{h['duration']} {h['severity']} {h['timestamp']}\n")
    
    print(f"\n✅ Saved to '{output_file}'")
    
    # Statistics
    print("\n📊 STATISTICS:")
    for htype in hazard_types:
        count = sum(1 for h in hazards if h['type'] == htype)
        print(f"  {htype}: {count}")
    print()


if __name__ == "__main__":
    print("="*60)
    print("HAZARD GENERATOR")
    print("="*60)
    print()
    
    # Light scenario
    generate_hazards(60, 40, 5, "hazards_light.txt")
    print()
    
    # Medium scenario
    generate_hazards(60, 40, 15, "hazards_medium.txt")
    print()
    
    # Heavy scenario
    generate_hazards(60, 40, 30, "hazards_heavy.txt")
    
    print("="*60)
    print("✅ All scenarios generated!")
    print("="*60)

