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
#include "Mona/UDProtocol.h"

namespace Mona {

class DNSHelper;
class DNSProtocol : public UDProtocol, public virtual Object  {
public:
	DNSProtocol(const char* name, Invoker& invoker, Sessions& sessions);
    ~DNSProtocol();
	
	bool        load(Exception& ex,const SocketAddress& address);
private:
	void		manage() {; }
private:
	UDProtocol::OnPacket::Type onPacket;
    std::unique_ptr<DNSHelper>	_pDNSHelper;;
};


} // namespace Mona
