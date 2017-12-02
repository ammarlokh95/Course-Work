JFLAGS = -g
JC = javac
#
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java
CLASSES = \
        *.java \
default: classes
classes: $(CLASSES:.java=.class)
clean:
	$(RM) *.class

test1:
	java AirportSim.class
test2:
	java AirportSim.class 4 1 4 2
test3:
	java AirportSim.class 2 1 3 1
test4:
	java AirportSim.class 3 1 7 4
test5:
	java AirportSim.class 4 1 5 4
