with open('a', 'r') as f:
    for line in f:
        if line.find("sdd size") != -1:
        	print line,