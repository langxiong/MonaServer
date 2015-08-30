/*
Copyright 2014 Mona
lang.xiong.sc[a]gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License received along this program for more
details (or else see http://www.gnu.org/licenses/).

This file is a part of Mona.
*/

#include "Mona/DNS/DNS.h"
#include "Mona/Crypto.h"
#include "Mona/Logs.h"
#include <WinSock2.h>
#include <windows.h>
#include <windns.h> 

using namespace std;
#pragma comment(lib, "Dnsapi.lib")

namespace Mona {

    typedef int (CALLBACK* DNSFLUSHPROC)();
    typedef int (CALLBACK* DHCPNOTIFYPROC)(LPWSTR, LPWSTR, BOOL, DWORD, DWORD, DWORD, int);

    std::string DNS::GetHostFromPacket(const char* pBuf, size_t sz)
    {
        if (sz < sizeof(DNS_MESSAGE_BUFFER))
        {
            throw std::exception("invalid packet size.");
        }

        std::string host;
        PDNS_MESSAGE_BUFFER pDNSMsgBuf = reinterpret_cast<PDNS_MESSAGE_BUFFER>(const_cast<char*>(pBuf));
        if (pDNSMsgBuf->MessageBody)
        {
            int i = 0;
            int count = pDNSMsgBuf->MessageBody[i++];
            while (count--)
            {
                host.append(1, pDNSMsgBuf->MessageBody[i++]);
            }
            while ((count = pDNSMsgBuf->MessageBody[i++]) != 0)
            {
                host.append(1, '.');
                while (count--)
                {
                    host.append(1, pDNSMsgBuf->MessageBody[i++]);
                }
            }
            INFO("Query host ", host);
        }
        return host;
    }

    std::string DNS::GetResponseIPs(const char* pBuf, size_t sz)
    {
        if (sz < sizeof(DNS_MESSAGE_BUFFER))
        {
            throw std::exception("invalid packet size.");
        }

        std::string host;
        PDNS_MESSAGE_BUFFER pDNSMsgBuf = reinterpret_cast<PDNS_MESSAGE_BUFFER>(const_cast<char*>(pBuf));
        if (pDNSMsgBuf->MessageBody)
        {
            int i = 0;
            int count = pDNSMsgBuf->MessageBody[i++];
            while (count--)
            {
                host.append(1, pDNSMsgBuf->MessageBody[i++]);
            }
            while ((count = pDNSMsgBuf->MessageBody[i++]) != 0)
            {
                host.append(1, '.');
                while (count--)
                {
                    host.append(1, pDNSMsgBuf->MessageBody[i++]);
                }
            }
            INFO("Response host ", host);
        }
        PDNS_RECORD pRecord = NULL;
        DNS_BYTE_FLIP_HEADER_COUNTS(&pDNSMsgBuf->MessageHead);
        auto s = DnsExtractRecordsFromMessage_W(pDNSMsgBuf, sz, &pRecord);
        if (pRecord)
        {
            PDNS_RECORD pRecordData = pRecord;
            while (pRecordData)
            {
                if (std::string(pRecordData->pName) != ".")
                {
                    INFO("Response record ", pRecordData->pName);
                }
                pRecordData = pRecordData->pNext;
            }
            DnsRecordListFree(pRecord, DnsFreeRecordList);
        }
        DNS_BYTE_FLIP_HEADER_COUNTS(&pDNSMsgBuf->MessageHead);
        return host;
    }

    bool DNS::FlushDNS()
    {
        auto hDll = ::LoadLibraryA("dnsapi");
        if (!hDll)
        {
            return false;
        }
        bool isSuccess = false;
        DNSFLUSHPROC pDNSFlushProc = (DNSFLUSHPROC)::GetProcAddress(hDll, "DnsFlushResolverCache");
        if (pDNSFlushProc)
        {
            isSuccess = true;
            auto ret = pDNSFlushProc();
            isSuccess = true;
        }
        ::FreeLibrary(hDll);
        return isSuccess;
    }

    bool DNS::NotifyIPChanged(const std::string& strAdapterName)
    {
        bool isSuccess = false;
        HINSTANCE hDhcpDll;
        DHCPNOTIFYPROC	pDhcpNotifyProc;
        WCHAR wcAdapterName[256] = { 0 };
        ::MultiByteToWideChar(CP_UTF8, 0, strAdapterName.c_str(), -1, wcAdapterName, 256);
        if ((hDhcpDll = LoadLibraryA("dhcpcsvc")) == NULL)
            return false;

        if ((pDhcpNotifyProc = (DHCPNOTIFYPROC) GetProcAddress(hDhcpDll, "DhcpNotifyConfigChange")) != NULL)
            if ((pDhcpNotifyProc) (NULL, wcAdapterName, FALSE, 0, NULL, NULL, 0) == ERROR_SUCCESS)
                isSuccess = true;

        FreeLibrary(hDhcpDll);
        return isSuccess;
    }


}  // namespace Mona
