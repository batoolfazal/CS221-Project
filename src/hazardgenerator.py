"""
Random Hazard Generator for Drone Flight Simulation
Generates dynamic hazards that appear during mission
"""

import random
import time

def generate_dynamic_hazards(map_width, map_height, num_hazards, output_file):
    """
    Generate random hazards for testing
    
    Args:
        map_width: Width of the map
        map_height: Height of the map
        num_hazards: Number of hazards to generate
        output_file: Output filename
    """
    
    hazard_types = ["BIRD", "DRONE", "WEATHER", "DEBRIS", "AIRCRAFT"]
    
    hazards = []
    
    print(f"🎲 Generating {num_hazards} random hazards...")
    print()
    
    for i in range(num_hazards):
        # Random position
        x = random.randint(0, map_width - 1)
        y = random.randint(0, map_height - 1)
        
        # Random type
        hazard_type = random.choice(hazard_types)
        
        # Duration based on type
        if hazard_type == "BIRD":
            duration = random.randint(5, 20)      # 5-20 seconds
            severity = random.randint(2, 4)
        elif hazard_type == "DRONE":
            duration = random.randint(10, 30)     # 10-30 seconds
            severity = random.randint(3, 5)
        elif hazard_type == "WEATHER":
            duration = random.randint(30, 60)     # 30-60 seconds
            severity = random.randint(1, 3)
        elif hazard_type == "DEBRIS":
            duration = random.randint(15, 45)     # 15-45 seconds
            severity = random.randint(3, 5)
        else:  # AIRCRAFT
            duration = random.randint(20, 40)     # 20-40 seconds
            severity = 5                          # Always critical
        
        # Timestamp (when detected)
        timestamp = int(time.time()) + random.randint(0, 300)  # Within 5 minutes
        
        hazards.append({
            'x': x,
            'y': y,
            'type': hazard_type,
            'duration': duration,
            'severity': severity,
            'timestamp': timestamp
        })
        
        print(f"Hazard {i+1}: ({x},{y}) - {hazard_type} "
              f"[Severity: {severity}/5, Duration: {duration}s]")
    
    # Sort by timestamp
    hazards.sort(key=lambda h: h['timestamp'])
    
    # Save to file
    with open(output_file, 'w') as f:
        f.write(f"# Dynamic Hazards for Simulation\n")
        f.write(f"# Format: X Y TYPE DURATION SEVERITY TIMESTAMP\n")
        f.write(f"# Map size: {map_width}x{map_height}\n")
        f.write(f"# Total hazards: {num_hazards}\n\n")
        
        for h in hazards:
            f.write(f"{h['x']} {h['y']} {h['type']} {h['duration']} "
                   f"{h['severity']} {h['timestamp']}\n")
    
    print(f"\n✅ Hazards saved to '{output_file}'")
    
    # Generate statistics
    print("\n📊 HAZARD STATISTICS:")
    type_counts = {}
    for h in hazards:
        t = h['type']
        type_counts[t] = type_counts.get(t, 0) + 1
    
    for hazard_type, count in sorted(type_counts.items()):
        print(f"  {hazard_type}: {count}")
    
    print()


def generate_scenario_hazards():
    """Generate specific scenarios for testing"""
    
    print("="*60)
    print("SCENARIO HAZARD GENERATOR")
    print("="*60)
    print()
    
    # Scenario 1: Light hazard density
    print("📋 Scenario 1: Light Hazard Density")
    generate_dynamic_hazards(60, 40, 5, "hazards_light.txt")
    
    print("\n" + "="*60 + "\n")
    
    # Scenario 2: Medium hazard density
    print("📋 Scenario 2: Medium Hazard Density")
    generate_dynamic_hazards(60, 40, 15, "hazards_medium.txt")
    
    print("\n" + "="*60 + "\n")
    
    # Scenario 3: Heavy hazard density
    print("📋 Scenario 3: Heavy Hazard Density")
    generate_dynamic_hazards(60, 40, 30, "hazards_heavy.txt")
    
    print("\n" + "="*60)
    print("✅ All scenario files generated!")
    print("📁 Files created:")
    print("   - hazards_light.txt (5 hazards)")
    print("   - hazards_medium.txt (15 hazards)")
    print("   - hazards_heavy.txt (30 hazards)")
    print("="*60)


if __name__ == "__main__":
    generate_scenario_hazards()
