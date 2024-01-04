import glob
import shutil
import os

EXE = "EXE"
ACS = "ACS"
os.makedirs(EXE, exist_ok=True)
files = glob.glob('EXE/*'+".txt", recursive=True)
print(files)
for f in files:
    try:
        os.remove(f)
    except OSError as e:
        print("Error: %s : %s" % (f, e.strerror))


def getNames(dir):
    files = glob.glob(dir+'/*.txt', recursive=True)
    temp = []
    names = []

    for f in files:
        temp.append(f.split("\\")[-1])

    for i in range(len(temp)):
        names.append(temp[i])
    return names


Points_en = False
p_z = [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6]
for name in getNames(ACS):
    if name.find("acs_bc1_T0_293.txt") == -1:
        continue
    id = 1
    source = ACS+"/"+name
    file_r = open(source, 'r')
    lines = file_r.readlines()
    for p_zi in p_z:
        name_i = name+"_"+str(id)
        id = id+1
        dest = EXE+"/"+name_i
        #shutil.copy(source, dest)
        for i in range(len(lines)):
            if lines[i].find("Points") != -1:
                Points_en = True
            if lines[i].find("z = ") != -1 and Points_en:
                lines[i] = "        z = "+str(p_zi)+"\n"
                Points_en = False
        file_w = open(dest, 'w')
        file_w.writelines(lines)
