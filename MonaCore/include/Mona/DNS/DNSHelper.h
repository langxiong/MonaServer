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

#include <vector>
#include <map>
#include <string>
#include <IPHlpApi.h>

namespace Mona
{
	class DNSHelper
	{
		static const std::string sm_RootKey;
	public:
        DNSHelper(const std::string& blockListFilePath);
        ~DNSHelper();

	public:
		const FIXED_INFO* GetFixedInfo() const;
        std::string GetDNSServerAddress(const std::string& host) const;
		bool ChangeDefaultAdpaterDNSSetting();
		bool ResetDefaultAdapterDNSSetting();
	private:
        static std::vector<std::string> GetPerAdapterNameServers(const PIP_PER_ADAPTER_INFO pPerAdapterInfo);
        void Init();
		void GetAdapterInfos();
        bool IsBlockHost(const std::string& host) const;

    private:
		FIXED_INFO* m_pFixInfo;
        std::string m_blockListFilePath;
		std::vector<std::string> m_DNSServers;
        std::vector<std::string> m_blockList;
		std::map<std::string, std::vector<std::string>> m_mapNameServers;
	};
} // YC