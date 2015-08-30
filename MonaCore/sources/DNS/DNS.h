/******************************************************************************
 *  版权所有（C）2010-2015，上海游创网络科技有限公司	                      *
 *  保留所有权利。                                                            *
 ******************************************************************************
 *  作者 : 熊浪
 *  版本 : 1.0
 *****************************************************************************/
/*  修改记录: 
      日期       版本    修改人             修改内容
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