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

#include "Mona/DNS/DNSProtocol.h"
#include "Mona/DNS/DNS.h"
#include "Mona/DNS/DNSSession.h"


using namespace std;


namespace Mona {


DNSProtocol::DNSProtocol(const char* name, Invoker& invoker, Sessions& sessions) : UDProtocol(name, invoker, sessions) {

	// timesBeforeTurn, no by default

	onPacket = [this](PoolBuffer& pBuffer,const SocketAddress& address) {
        this->sessions.create<DNSSession>(*this, this->invoker, pBuffer, address);
	};

	OnPacket::subscribe(onPacket);
}

DNSProtocol::~DNSProtocol() {
	OnPacket::unsubscribe(onPacket);
}

bool DNSProtocol::load(Exception& ex, const SocketAddress& address) {

	if (!UDProtocol::load(ex,address))
		return false;
	return true;
}



} // namespace Mona
