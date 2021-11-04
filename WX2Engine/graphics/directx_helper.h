/*********************************************************************
 * @file	input_layout.h
 * @author	SleepingDog0529
 * @date	2021/07/11 19:54
 ********************************************************************/
#pragma once

namespace wx2
{
	/**
	 * @brief     ID3DBlob�̕��������string�ɕϊ�����
	 * @param[in] blob ��������
	 * @return    string ������
	 */
	std::string BlobMessageToString(ID3DBlob* blob);
}