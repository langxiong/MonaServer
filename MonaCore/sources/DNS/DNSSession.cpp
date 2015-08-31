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

#include "Mona/DNS/DNSSession.h"
#include "Mona/DNS/DNSProtocol.h"

using namespace std;

namespace Mona {

DNSSession::DNSSession(DNSProtocol& protocol,
    Invoker& invoker, PoolBuffer& pBuffer, const SocketAddress& peerAddress, const SocketAddress& DNSServerAddress) : Session(protocol, invoker, "DNS:Resovler"),
    _peerAddress(peerAddress),
    _remoteUDPResoverAddr(invoker.sockets, *this)
{
    Exception e;
    _remoteUDPResoverAddr.send(e, pBuffer->data(), pBuffer->size(), DNSServerAddress);
}

const SocketAddress& DNSSession::PeerAddress() const
{
    return _peerAddress;
}

void DNSSession::kill(UInt32 type /*= NORMAL_DEATH*/)
{
    _remoteUDPResoverAddr.close();
    Session::kill(type);
}

} // namespace Mona
