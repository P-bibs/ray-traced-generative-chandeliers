import sys
from components.Scene import Scene


# Get the output file from command line argument
if len(sys.argv) < 2:
    print("Usage: python main.py <output file>")
    sys.exit(1)

output_file = sys.argv[1]

# Generate the scene file
scene = Scene()
scene_file = scene.scene_rep()

# open file for writing
with open(output_file, "w") as f:
    f.write(scene_file)

