
#include <obdPacket.hpp>
using OBDPacket::PacketType;
using OBDPacket::ValueType;
#define PACKET_SIZE 10
OBDPacket::OBDPacket(const char* buffer, int size)
{
	if(size == PACKET_SIZE)
	{
	// Assume Valid Packet
		int tempType = buffer[0] - '0';
		if(tempType > 0 && tempType < 10)
			switch(tempType)
			{
			 case PacketType::value: pType = PacketType::value;
				int tempValType = buffer[1] - '0';
				double* tempVal = buffer + 2; 
				switch(tempValType)
				{
				 case ValueType::speed: value = *tempVal; break;
				 case ValueType::fuel: value = *tempVal; break;
				 case ValueType::rpm: value = *tempVal; break;
				 case ValueType::odo: value = *tempVal; break;
				 default: break;
				} break;
			case PacketType::error: pType = PacketType::error;
				code = strcpy_s(code, &buffer[2], 8);
				break;
			case PacketType::message: pType = PacketType::message;
				code = strcpy_s(code, &buffer[2], 8);
				break;
		}
	}
}
