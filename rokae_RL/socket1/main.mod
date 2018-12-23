MODULE MainModule
LOCAL VAR jointtarget j1 = j:{12.30,31.25,-44.32,0.53,99,60}{EJ 0.000000000,0.000000000,0.000000000,0.000000000,0.000000000,0.000000000}
LOCAL VAR double double1[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0}
LOCAL VAR robtarget p1 = p:{{464.971213901,91.699416555,486.566718887},{0.012917599,0.392652317,0.919237261,0.025693410}}{cfg 0,0,0,0}{EJ 0.000000000,0.000000000,0.000000000,0.000000000,0.000000000,0.000000000}
LOCAL VAR string string1 = "I am a string."
GLOBAL VAR robtarget p2 = p:{{445.315310095,126.862911911,498.835261299},{0.007132725,0.389587655,0.920471354,0.030051129}}{cfg 0,0,0,0}{EJ 0.000000000,0.000000000,0.000000000,0.000000000,0.000000000,0.000000000}
GLOBAL VAR double double2[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0}
GLOBAL VAR byte byte1[2] = {0,0}
GLOBAL VAR int int1 = 0

GLOBAL PROC main()
	MoveAbsJ j1,v1000,z50,tool0,wobj01
	SocketClose(slisten)
	SocketCreate("192.168.2.108",8080,slisten)
	Print("I have connected to a server!")
	SocketSendString("hello",slisten)
//	SocketSendByte(byte1,slisten)
	Print("I have send a data!")
//	string1="I am a string."
//	string1=SocketReadString(60,slisten)
//	Print(string1)
	double1=SocketReadDouble(7,60,slisten)
	Print(double1[1],",",double1[2],",",double1[3],",",double1[4],",",double1[5],",",double1[6],",",double1[7])
	p1.x=double1[1]
	p1.y=double1[2]
	p1.z=double1[3]
	MoveL p1,v1000,z50,tool0,wobj0
	p2=CRobT(tool0,wobj0)
    double2[1]=p2.x
	double2[2]=p2.y
	double2[3]=p2.z
	double2[4]=p2.q1
	double2[5]=p2.q2
	double2[6]=p2.q3
	double2[7]=p2.q4
	Print(double2[1],",",double2[2],",",double2[3],",",double2[4],",",double2[5],",",double2[6],",",double2[7])
	SocketSendString(" ",slisten)
//	string1=DoubleToStr(double2[1],2)
//	SocketSendString(string1,slisten)
	int1=1
	WHILE(int1<8)
		string1=DoubleToStr(double2[int1],2)
		SocketSendString(string1,slisten)
		SocketSendString(" ",slisten)
		int1++
	ENDWHILE
ENDPROC
ENDMODULE