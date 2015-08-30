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

#include "Mona/BlindlyProxy/TunnelSession.h"

using namespace std;

namespace Mona {

TunnelSession::TunnelSession(ProxyProtocol& protocol,
    Invoker& invoker, PoolBuffer& pBuffer, const SocketAddress& peerAddress) : Session(protocol, invoker, "Tunnel:Resovler")
{
    {
       /* _onTunnelEstablish = [this](PoolBuffer& poolBuffer, const SocketAddress& addr)
        {

        };*/
    }
    Exception e;
   // _socket.connect(e, peerAddress);
}

void TunnelSession::kill(UInt32 type /*= NORMAL_DEATH*/)
{
    Session::kill(type);
}

} // namespace Mona
