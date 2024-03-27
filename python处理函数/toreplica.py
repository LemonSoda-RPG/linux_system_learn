import os
import shutil

frames_path = " "
depths_path = " "
traj_path = " "

with open(traj_path,'r') as traj:
    trajlines = traj.readlines()
    num = 0
    for trajline in trajlines:
        formatted_num = "{:06}".format(num)
        trajline = trajline.strip()
        trajline = trajline.split()
        id = trajline[0]
        framepath = os.path.join(frames_path,id+".png")
        depthpath = os.path.join(depths_path,id+".png")
        newframepath = "results/frame"+formatted_num+".png"
        newdepthpath = "results/depth"+formatted_num+".jpg"
        shutil.copy2(depthpath,newdepthpath)
        shutil.copy2(framepath,newframepath)



file_path = "file.txt"  # 文本文件的路径
# 打开文本文件并逐行读取并修改内容
with open(file_path, "r") as file:
    lines = file.readlines()  # 读取所有行文本，并存储在列表中

# 逐行修改内容
modified_lines = []
for line in lines:
    modified_line = line.split(" ", 1)[1]  # 从第一个空格之后截取内容
    modified_lines.append(modified_line)

# 将修改后的内容写入文本文件
with open(file_path, "w") as file:
    file.writelines(modified_lines)