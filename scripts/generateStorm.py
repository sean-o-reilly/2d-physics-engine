# This script generates an environment json file named "storm" with a large platform and n dynamic bodies above it.

import random

PLATFORM_WIDTH = 20000
N = 500

# create an environment json file with a large platform
with open('saves/environments/storm.json', 'w') as f:
	f.write("{\n")
	f.write('  "StaticBody":\n')
	f.write("  [\n")
	f.write('    {"x": -10,"y": -10,"width": 20,"height": 20},\n')  # small box in center for reference
	f.write('    {"x": -10000,"y": 100,"width": ' + str(PLATFORM_WIDTH) + ',"height": 100}\n') # ground/platform
	f.write("  ],\n")
	f.write('  "DynamicBody":\n')
	f.write("  [\n")
	
minX = PLATFORM_WIDTH // -2
maxX = PLATFORM_WIDTH // 2
maxY = -500 
minY = -8000 # negative heights are higher up

for i in range(0, N): # loop will write n dynamic bodies to a the json file

    x = random.randint(minX, maxX)
    y = random.randint(minY, maxY)

    dynamicBody = '    {"x": ' + str(x) + ',"y": ' + str(y) + ',"width": 50,"height": 50}'

    if i < N - 1:
        dynamicBody += ",\n"
    else:
        dynamicBody += "\n"

    with open('saves/environments/storm.json', 'a') as f:
          f.write(dynamicBody)

with open('saves/environments/storm.json', 'a') as f:
    f.write("  ]\n")
    f.write("}")