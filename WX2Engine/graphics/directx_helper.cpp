#include "directx_helper.h"

namespace wx2 
{
	std::string BlobMessageToString(ID3DBlob* blob)
	{
		std::string msg(reinterpret_cast<const char*>(blob->GetBufferPointer()));
		return msg;
	}
}