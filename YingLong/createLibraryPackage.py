
import os
import shutil

print("Start create YingLong library for publish.")

libraryDirName = "YingLongStatic"
inlucdePath = os.path.join(libraryDirName, "include")
libPath = os.path.join(libraryDirName, "lib")
thirdPartyPath = os.path.join(libraryDirName, "thirdParty")

def ignoreNotHeaderFiles(dirPath, filelist):
    notHeaderFiles = [f for f in filelist if os.path.isfile(os.path.join(dirPath, f)) and os.path.splitext(f)[-1] != ".h"]
    notHeaderFiles.append("YingLongPCH.h")
    return notHeaderFiles

if os.path.exists(libraryDirName):
    shutil.rmtree(libraryDirName)

shutil.copytree("src", os.path.join(libraryDirName, "include"), ignore = ignoreNotHeaderFiles)
shutil.copytree("thirdParty", thirdPartyPath)
shutil.copytree("bin", libPath, ignore = lambda dirPath, filelist: [f for f in filelist if os.path.isfile(os.path.join(dirPath, f)) and os.path.splitext(f)[-1] not in (".lib", ".pdb")])
shutil.copyfile("dependencies.lua", os.path.join(libraryDirName, "dependencies.lua"))

print("Finish create YingLong library for publish.")

print("Start copy YingLongStatic to test.")

testYingLongLibPath = os.path.join("..", "test", "thirdParty", libraryDirName)

if os.path.exists(testYingLongLibPath):
    shutil.rmtree(testYingLongLibPath)

shutil.copytree(libraryDirName, testYingLongLibPath)

print("Finish copy YingLongStatic to test.")