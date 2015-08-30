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
        DNSHelper();
        ~DNSHelper();

	public:
		const FIXED_INFO* GetFixedInfo() const;
		const std::vector<std::string>& GetDefaultDNSServers() const;
		bool ChangeDefaultAdpaterDNSSetting();
		bool ResetDefaultAdapterDNSSetting();
	private:
		void Init();
		void GetAdapterInfos();
		static std::vector<std::string> GetPerAdapterNameServers(const PIP_PER_ADAPTER_INFO pPerAdapterInfo);
	private:
		FIXED_INFO* m_pFixInfo;
		std::vector<std::string> m_DNSServers;
		std::map<std::string, std::vector<std::string>> m_mapNameServers;
	};
} // YC