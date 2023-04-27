#!/usr/bin/env python

import json

with open('.config', mode = 'r') as configFile:
	for line in configFile:
		key, value = line.replace('export ', '', 1).strip().split('=', 1)
		if (key == 'BUILD_NAME'):
			buildName = value
# print(buildName)

rtconfig = []
with open('router/.config', mode = 'r') as rtconfigFile:
	for line in rtconfigFile:
		if line.startswith('#') or not line.strip():
			continue
		key, value = line.strip().split('=', 1)
		rtconfig.append(key)
# print(rtconfig)

jffsRequireSize = 0
p1Size = 0
p2Size = 0
p3Size = 0
addedRtconfig = []

with open("router/rom/rom/jffs.json") as jsonFile:
	jsonObject = json.load(jsonFile)
	jsonFile.close()

jffsArray = jsonObject['jffs']
# print("+"+"-"*62+"+")
for model in jffsArray:
	productid = model['productid']
	if (productid != 'general' and productid != buildName):
		continue
	# print("productid: " + productid)
	featureArray = model['features']
	if (featureArray):
		for feature in featureArray:
			featureName = feature['name']
			featureRtconfig = feature['rtconfig']
			if (str(featureRtconfig).startswith('NOCONFIG') and featureRtconfig not in rtconfig):
				rtconfig.append(featureRtconfig)
			if (featureRtconfig in rtconfig and featureRtconfig not in addedRtconfig):
				# print("| %-61s|" % featureName)
				# print("+"+"-"*3+"+"+"-"*13+"+"+"-"*44+"+")
				addedRtconfig.append(featureRtconfig)
				featureDataArray = feature['data']
				if (featureDataArray):
					for data in featureDataArray:
						dataName = data['name']
						dataSize = data['max_size']
						dataPriority = data['priority']
						# print("| %d | " % dataPriority + "%8d " % int(dataSize/1024) + "KB | %-43s|" % dataName)
						jffsRequireSize += dataSize
						if (dataPriority == 1):
							p1Size += dataSize
						elif (dataPriority == 2):
							p2Size += dataSize
						elif (dataPriority == 3):
							p3Size += dataSize
					# print("+"+"-"*3+"+"+"-"*13+"+"+"-"*44+"+")

print("Require JFFS: %6.2f MB" % float(float(jffsRequireSize)/1048576))
# print("  Priority 1: %6.2f MB" % float(float(p1Size)/1048576))
# print("  Priority 2: %6.2f MB" % float(float(p2Size)/1048576))
# print("  Priority 3: %6.2f MB" % float(float(p3Size)/1048576))

with open("router/jffs_requirement.txt", 'w') as out_file:
	out_file.write("%d" % jffsRequireSize)
