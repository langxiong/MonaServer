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

#pragma once

#include "Mona/Mona.h"
#include "Mona/Session.h"
#include "Mona/UDPSocket.h"
#include "Mona/Time.h"

namespace Mona {

class DNSProtocol;
class DNSSession : public Session, public virtual Object {
    class DNSSocket : public UDPSocket
    {
    public:
        DNSSocket(const SocketManager& manager, DNSSession& session) :UDPSocket(manager), _session(session)
        {
            onPacket = [this](PoolBuffer& pBuffer, const SocketAddress& address)
            {
                Exception e;
                this->send(e, pBuffer->data(), pBuffer->size(), this->_session.PeerAddress());
                this->_session.kill();
            };

            OnPacket::subscribe(onPacket);
        }
        ~DNSSocket()
        {
            OnPacket::unsubscribe(onPacket);
        }

        OnPacket::Type onPacket;
    private:

        DNSSession& _session;
    };
public:
	DNSSession(DNSProtocol& protocol,
        Invoker& invoker, PoolBuffer& pBuffer, const SocketAddress& peerAddress);

    const SocketAddress& PeerAddress() const;

    virtual void kill(UInt32 type = NORMAL_DEATH);

private:

    SocketAddress _peerAddress;
    DNSSocket _remoteUDPResoverAddr;
};


} // namespace Mona
