//---------------------------------------------------------------------------

#ifndef RS232DevH
#define RS232DevH
//---------------------------------------------------------------------------
class RS232Device
{
   protected:
		int Port;
		char* command;
		char* response;
		char* ErrorString;
		int Error;
		int MaxLength;
	public:
		RS232Device(int _Port);
		~RS232Device();
		int Reset(int _Port, int _ML = 256);
		int SetBaud(int _Baud=9600);
		int GetPort() {return Port;}
		int InitPort(int tbuf = 256, int rBuf = 256);
		int FreePort();
		int Send(int delay=20);
		int Read(int delay=1000);
		int virtual Test();
};
#endif
