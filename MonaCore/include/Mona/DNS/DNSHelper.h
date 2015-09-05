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