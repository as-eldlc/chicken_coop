#!/usr/bin/python

from datetime import datetime

now = datetime.now()

#define HOUR 16
#define MINUTE 37
#define MONTH 3
#define YEAR 2018
#define DAY 27
#define DAY_OF_WEEK 2
output = ""
output += "#define HOUR %d" % now.hour + "\n"
output += "#define MINUTE %d" % now.minute + "\n"
output += "#define YEAR %d" % now.year + "\n"
output += "#define MONTH %d" % now.month + "\n"
output += "#define DAY %d" % now.day + "\n"
output += "#define DAY_OF_WEEK %d" % (now.weekday()+1) + "\n"

f = open("./src/Date.h","w")
f.write(output)
f.close()
