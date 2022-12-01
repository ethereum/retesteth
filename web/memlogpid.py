#https://askubuntu.com/questions/56266/how-to-log-memory-and-cpu-usage-of-an-application
import os
import sys
import time
import string
import commands
import subprocess

def get_cpumem(pid):
    d = commands.getoutput("ps -p " + str(pid) + " --no-headers --format \"%cpu %mem\"")
    if d == "":
        return (None, None)
    return (float(d.split()[0]), float(d.split()[1])) if d else (None, None)
    #d = [i for i in commands.getoutput("ps aux").split("\n")
    #    if i.split()[1] == str(pid)]
    #return (float(d[0].split()[2]), float(d[0].split()[3])) if d else None

if __name__ == '__main__':
    #if not len(sys.argv) == 2 or not all(i in string.digits for i in sys.argv[1]):
    #    print("usage: %s PID" % sys.argv[0])
    #    exit(2)
    #pid = os.system(sys.argv[1])

    argCommand = sys.argv[1]
    argMemLogFile = sys.argv[2]
    argLogFile = sys.argv[3]

    my_env = os.environ.copy()
    my_env["JAVA_HOME"] = "/usr/lib/jvm/java-11-openjdk-amd64"

    logfile = open(argLogFile, 'w')
    proc = subprocess.Popen([argCommand], shell=True, stdout=logfile, stderr=logfile, env = my_env) 

    time.sleep(4)
    file1 = open(argMemLogFile, "a")
    i = 0
    try:
        while True:
            file1.close()
            file1 = open(argMemLogFile, "a")

            x,y = get_cpumem(proc.pid + 1)
            if x == None:            
                print("no such process")
                logfile.close()
                file1.close()
                exit(1)
            #print("%.2f\t%.2f" % (x,y * 15300 / 100))
            #print x, y
            file1.write("%d\t%.2f\t%.2f" % (i,y * 7680 / 100, x))
            file1.write("\n")
            file1.close()
            i = i + 1
            time.sleep(10)
    except KeyboardInterrupt:
        print
        file1.close()
        logfile.close()
        exit(0)
    except:
        file1.close()
        logfile.close()
        exit(0)
#set xtics textcolor rgb 0xffffff;
#gnuplot -e "set terminal png size 400,300; set output 'xyz.png'; plot 'log.txt' w linespoints pt 7"
#gnuplot -e "set terminal png background 0x000000   size 1024,200; set output 'xyz.png'; plot '2021-09-23-1632393183-MEMt8ntool.txt' w linespoints pt 6 lc rgb 0xbfff00"
#gnuplot -e "set key textcolor rgb 0xffffff; set title textcolor rgb 0xffffff; set border linecolor rgb 0xffffff;  set terminal png background 0x000000   size 1024,200; set output 'xyz.png'; plot '2021-09-23-1632393183-MEMt8ntool.txt' w linespoints pt 6 lc rgb 0xbfff00"
#gnuplot -e "set y2tics textcolor rgbcolor 0x4ac9ff; set ytics nomirror textcolor rgbcolor 0xbfff00; set key textcolor rgb 0xffffff; set title textcolor rgb 0xffffff; set border linecolor rgb 0xffffff;  set terminal png background 0x000000   size 1024,200; set output 'xyz.png'; set multiplot; plot 'log1.txt' using 1:2 title 'mem' axes x1y1 w linespoints pt 6 lc rgb 0xbfff00, 'log1.txt' using 1:3 title 'cpu' axes x1y2 w linespoints pt 6 lc rgb 0x4ac9ff;"
