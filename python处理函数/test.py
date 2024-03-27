with open("haha.txt",'r') as file:
    lines = file.readlines()
   
    for line in lines:
        line = line.strip()
        line = line.split()
        print(line[0])