/* 
	Copyright 2013 Mona - mathieu.poux[a]gmail.com
 
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
#include "Mona/Protocol.h"
#include "Mona/DatagramSocket.h"
#include "Mona/Logs.h"

namespace Mona {

class UDProtocol : public Protocol, public DatagramSocket, virtual Object {
protected:
	UDProtocol(const char* name, Invoker& invoker, Gateway& gateway) : DatagramSocket(invoker.sockets), Protocol(name, invoker, gateway) {}
	
	virtual bool load(Exception& ex, const ProtocolParams& params);
private:
	void			onReadable(Exception& ex) { gateway.readable(ex,*this); }
	void			onError(const std::string& error) { WARN("Protocol ",name,", ", error); }
};

inline bool UDProtocol::load(Exception& ex, const ProtocolParams& params) {
	SocketAddress address;
	address.set(IPAddress::Wildcard(), params.port);
	return bind(ex, address);
}


} // namespace Mona