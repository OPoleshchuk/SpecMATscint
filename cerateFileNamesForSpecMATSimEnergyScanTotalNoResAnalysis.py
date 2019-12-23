#Author: Oleksii Poleshchuk
#
#KU Leuven 2016-2019
#
#Part of SpecMATscint
#
# \file cerateFileNamesForSpecMATSimEnergyScanTotalNoResAnalysis.py
#This script is used for automatic file name generation used in
#the SpecMATSimEnergyScanTotalNoResAnalysis.C script
#

f = open("generatedFileNamesEnergyScan.txt", "w+")

def formatNumber(num):
  if num % 1 == 0:
    return int(num)
  else:
    return num


for i in range(20, 300, 10):
    j = i/1000
    j = formatNumber(j)
    print(j)
    f.write('convertRoot("CeBr3_48mmx48mmx48mm_20x1x1crystals_R173.628mm_gamma' + str(j) + 'MeV__wallThick_mm_point_sourcePosition_129mm__gap3mm", ' + str(j) + ', ' + str(i) + ',1);\n')
for i in range(300, 2000, 100):
    j = i/1000
    j = formatNumber(j)
    print(j)
    f.write('convertRoot("CeBr3_48mmx48mmx48mm_20x1x1crystals_R173.628mm_gamma' + str(j) + 'MeV__wallThick_mm_point_sourcePosition_129mm__gap3mm", ' + str(j) + ', ' + str(i) + ',1);\n')
for i in range(2000, 16000, 1000):
    j = i / 1000
    j = formatNumber(j)
    print(j)
    f.write('convertRoot("CeBr3_48mmx48mmx48mm_20x1x1crystals_R173.628mm_gamma' + str(j) + 'MeV__wallThick_mm_point_sourcePosition_129mm__gap3mm", ' + str(j) + ', ' + str(i) + ',1);\n')
f.close()
