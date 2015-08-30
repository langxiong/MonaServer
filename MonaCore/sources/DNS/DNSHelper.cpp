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
#include <winsock2.h>
#include <windows.h>

#include "Mona/DNS/DNSHelper.h"
#include "Mona/WinRegistryKey.h"
#include "Mona/Logs.h"
#include "Mona/DNS/DNS.h"

#pragma comment(lib, "Iphlpapi.lib")

namespace Mona
{
	DNSHelper::DNSHelper():
		m_pFixInfo(NULL)
	{
		Init();
	}

	DNSHelper::~DNSHelper()
	{
		if (m_pFixInfo)
		{
			std::free(m_pFixInfo);
			m_pFixInfo = NULL;
		}
		ResetDefaultAdapterDNSSetting();
	}

	void DNSHelper::Init()
	{
		if (m_pFixInfo)
		{
			return;
		}

		m_pFixInfo = (FIXED_INFO*)std::malloc(sizeof(FIXED_INFO));
		if (!m_pFixInfo)
		{
			throw std::exception("faild to alloc memoroy for fixed_info");
		}
		// Make an initial call to GetAdaptersInfo to get
		// the necessary size into the ulOutBufLen variable
		ULONG ulOutBufLen = sizeof(FIXED_INFO);
		if (GetNetworkParams(m_pFixInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
		{
			m_pFixInfo = (FIXED_INFO*)std::realloc(m_pFixInfo, ulOutBufLen);
			if (!m_pFixInfo)
			{
				throw std::exception("faild to realloc memoroy for fixed_info");
			}
		}
		auto ret = GetNetworkParams(m_pFixInfo, &ulOutBufLen);
		if (ret == NO_ERROR)
		{
            INFO("Host name: ", m_pFixInfo->HostName, " Domain name: ", m_pFixInfo->DomainName);
			m_DNSServers.push_back(m_pFixInfo->DnsServerList.IpAddress.String);
			for (auto pIpAddr = m_pFixInfo->DnsServerList.Next; pIpAddr != NULL; pIpAddr = pIpAddr->Next)
			{
				m_DNSServers.push_back(pIpAddr->IpAddress.String);
			}

			for (auto it = m_DNSServers.begin(); it != m_DNSServers.end(); ++it)
			{
                NOTE("DNS server: ", *it);
			}
		}
		GetAdapterInfos();
	}

	const FIXED_INFO* DNSHelper::GetFixedInfo() const
	{
		// return m_pFixInfo;
		return NULL;
	}

	void DNSHelper::GetAdapterInfos()
	{
		ULONG ulAdapterInfoSize = sizeof(IP_ADAPTER_INFO);
		PIP_ADAPTER_INFO pAdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(std::malloc(ulAdapterInfoSize));
		if (!pAdapterInfo)
		{
			throw std::exception("failed to alloc memory for pAdapterInfo");
		}
		if (::GetAdaptersInfo(pAdapterInfo, &ulAdapterInfoSize) == ERROR_BUFFER_OVERFLOW) // out of buff
		{
			pAdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(std::realloc(pAdapterInfo, ulAdapterInfoSize));
			if (!pAdapterInfo)
			{
				throw std::exception("failed to realloc memory for pAdapterInfo");
			}
		}
		if (::GetAdaptersInfo(pAdapterInfo, &ulAdapterInfoSize) == ERROR_SUCCESS)
		{
			for (auto p = pAdapterInfo; p != nullptr; p = p->Next)
			{
				// If type is etherent
				ULONG ulPerAdapterInfoSize = sizeof(IP_PER_ADAPTER_INFO);
				PIP_PER_ADAPTER_INFO pPerAdapterInfo = reinterpret_cast<PIP_PER_ADAPTER_INFO>(std::malloc(ulPerAdapterInfoSize));
				if (!pPerAdapterInfo)
				{
					break;
				}

				if (::GetPerAdapterInfo(p->Index, pPerAdapterInfo, &ulPerAdapterInfoSize) == ERROR_BUFFER_OVERFLOW)
				{
					pPerAdapterInfo = reinterpret_cast<PIP_PER_ADAPTER_INFO>(std::realloc(pPerAdapterInfo, ulPerAdapterInfoSize));
					if (!pAdapterInfo)
					{
						break;
					}
				}
						
				if (::GetPerAdapterInfo(p->Index, pPerAdapterInfo, &ulPerAdapterInfoSize) == ERROR_SUCCESS)
				{
					m_mapNameServers.insert(std::make_pair(p->AdapterName, GetPerAdapterNameServers(pPerAdapterInfo)));
				}
				std::free(pPerAdapterInfo);
			}
		}
		std::free(pAdapterInfo);
	}

	std::vector<std::string> DNSHelper::GetPerAdapterNameServers(const PIP_PER_ADAPTER_INFO pPerAdapterInfo)
	{
		std::vector<std::string> ret;
		if (!pPerAdapterInfo)
		{
			return std::move(ret);
		}

		ret.push_back(pPerAdapterInfo->DnsServerList.IpAddress.String);
		for (auto pIpAddr = pPerAdapterInfo->DnsServerList.Next; pIpAddr != NULL; pIpAddr = pIpAddr->Next)
		{
			ret.push_back(pIpAddr->IpAddress.String);
		}
		return std::move(ret);
	}

	bool DNSHelper::ChangeDefaultAdpaterDNSSetting()
	{
        for (auto it = m_mapNameServers.begin(); it != m_mapNameServers.end(); ++it)
        {
            if (it->second == m_DNSServers)
            {
                WinRegistryKey regKey(sm_RootKey + it->first);
                Exception e;
                return 
                    regKey.setString(e, "NameServer", "127.0.0.1") &&
                    DNS::NotifyIPChanged(it->first) &&
                    DNS::FlushDNS();
            }
        }
		return false;
	}

	bool DNSHelper::ResetDefaultAdapterDNSSetting()
	{
        for (auto it = m_mapNameServers.begin(); it != m_mapNameServers.end(); ++it)
        {
            if (it->second == m_DNSServers)
            {
                WinRegistryKey regKey(sm_RootKey + it->first);
                Exception e;
                return 
                    regKey.setString(e, "NameServer", "") &&
                    DNS::NotifyIPChanged(it->first) && 
                    DNS::FlushDNS();
            }
        }
		return false;
	}

	const std::vector<std::string>& DNSHelper::GetDefaultDNSServers() const
	{
		return m_DNSServers;
	}

	const std::string DNSHelper::sm_RootKey("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\");

} // YC
