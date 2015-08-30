/******************************************************************************
 *  ��Ȩ���У�C��2010-2015���Ϻ��δ�����Ƽ����޹�˾	                      *
 *  ��������Ȩ����                                                            *
 ******************************************************************************
 *  ���� : ����
 *  �汾 : 1.0
 *****************************************************************************/
/*  �޸ļ�¼: 
      ����       �汾    �޸���             �޸�����
    --------------------------------------------------------------------------
******************************************************************************/
#pragma once

#include "Poco/Timespan.h"
#include "Poco/Net/SocketAddress.h"

namespace YC
{
	class CDNS
	{
	public:
		enum
		{ 
			// 2 * seconds
			DNS_TIMEOUT = 5 * 1000 * 1000,
		};
		CDNS();
		~CDNS();
	
	public:
		
		static std::string GetHostFromPacket(const char* pBuf, size_t sz);

		static std::string GetResponseIPs(const char* pBuf, size_t sz);

		static bool FlushDNS();

		static bool NotifyIPChanged(const std::string& strAdapterName);

	private:

		static std::string GenerateL2TPHello();

	};
} // YC