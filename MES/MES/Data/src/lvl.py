import glob
import shutil
import os

EXE = "EXE"
MES = "MES"
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


lvl = range(0, 7)
for name in getNames(MES):
    if name.find("ts") != -1:
        continue
    id = 1
    name_split = name.split("_")
    source = MES+"/"+name
    file_r = open(source, 'r')
    lines = file_r.readlines()
    for lvli in lvl:
        name_i = name_split[0]+str(id)+"_"+name_split[1]
        id = id+1
        dest = EXE+"/"+name_i
        #shutil.copy(source, dest)
        for i in range(len(lines)):
            if lines[i].find("lvl_z") != -1:
                lines[i] = "	lvl_z = "+str(lvli)+"\n"
        file_w = open(dest, 'w')
        file_w.writelines(lines)
