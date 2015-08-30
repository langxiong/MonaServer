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

#include "Mona/BlindlyProxy/ProxyProtocol.h"
#include "Mona/BlindlyProxy/Proxy.h"
#include "Mona/BlindlyProxy/TunnelSession.h"

using namespace std;


namespace Mona {


ProxyProtocol::ProxyProtocol(const char* name, Invoker& invoker, Sessions& sessions) : TCProtocol(name, invoker, sessions) {

	// timesBeforeTurn, no by default
    /*
        onPacket = [this](PoolBuffer& pBuffer,const SocketAddress& address) {
        this->sessions.create<DNSSession>(*this, this->invoker, pBuffer, address);
        };

        OnPacket::subscribe(onPacket);*/
}

ProxyProtocol::~ProxyProtocol() {
	// OnPacket::unsubscribe(onPacket);
}

bool ProxyProtocol::load(Exception& ex, const SocketAddress& address) {

	if (!TCProtocol::load(ex,address))
		return false;
	return true;
}



} // namespace Mona
