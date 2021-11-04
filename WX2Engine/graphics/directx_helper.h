/*********************************************************************
 * @file	input_layout.h
 * @author	SleepingDog0529
 * @date	2021/07/11 19:54
 ********************************************************************/
#pragma once

namespace wx2
{
	/**
	 * @brief     ID3DBlob‚Ì•¶š—ñî•ñ‚ğstring‚É•ÏŠ·‚·‚é
	 * @param[in] blob •¶š—ñî•ñ
	 * @return    string •¶š—ñ
	 */
	std::string BlobMessageToString(ID3DBlob* blob);
}