#Author: Oleksii Poleshchuk
#
#KU Leuven 2016-2019
#
#Part of SpecMATscint
#
# \file cerateFileNamesForSpecMATSimPositionScanTotalNoResAnalysis.py
#This script is used for automatic file name generation used in
#the SpecMATSimPositionScanTotalNoResAnalysis.C script
#

f = open("generatedFileNamesPositionScan.txt", "w+")

numberOfScans = 325

for i in range(0, numberOfScans):
    f.write('convertRoot("CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma1.33249MeV_Aluminum5083_Chamber_wallThick_3mm_point_sourcePosition_'+ str(i) + 'mm_FCEpoxy_epoxy_gap3mm", '+ str(i) + ', 1);\n')
f.close()
