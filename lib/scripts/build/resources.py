# Write your code here :-)
import os
path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
print(path)
path += "/../resources/"
file = open(path + "resources.qrc","w")
file.write("<RCC>\n")
file.write("\t<qresource prefix=\"/\">\n")
files = os.listdir(path)
for f in files:
    fStr  = str(f)
    print(fStr)
    if(fStr.find(".") < 0):
        files1 = os.listdir(path + fStr + "/")
        for f1 in files1:
            f1Str = str(f1)
            print(fStr + "/" + f1Str)
            if(f1Str.find(".") < 0):
                files2 = os.listdir(path + fStr + "/" + f1Str + "/")
                for f2 in files2:
                    f2Str = str(f2)
                    if(f2Str.find(".txt")>0 or f2Str.find(".pdf")>0 or f2Str.find(".html")>0):
                        print("Skipping unnecessary files. . .")
                    else:
                        print(fStr + "/" + f1Str + "/" + f2Str)
                        file.write("\t\t<file>" + fStr + "/" + f1Str + "/" + f2Str + "</file>\n")
            else:
               file.write("\t\t<file>" + fStr + "/" + f1Str + "</file>\n")
    elif (fStr == "resources.qrc"):
        print("Skipping resources file. . .\n")
    else:
        file.write("\t\t<file>" + f + "</file>\n")

file.write("\t</qresource>\n")
file.write("</RCC>\n")
file.close()