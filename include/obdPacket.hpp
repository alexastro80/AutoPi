#ifndef OBD_PACKET_HPP
#define OBD_PACKET_HPP

#define CODE_SIZE 6
#define PACKET_SIZE 10
class OBDPacket
{
public:
	OBDPacket(const char* buffer = nullptr, int size = 0);
	~OBDPacket()
		{ if(code != nullptr) delete code; }
	enum class PacketType {value,message,error,request};
	enum class ValueType {rpm, speed, fuel, odo};
	 
	double Value() { return value; }
	double Type() { return type; }
	
private:
	double value;
	char code[] = {"\0\0\0\0\0\0"};
	PacketType pType;
	ValueType vType;
};


#endif
