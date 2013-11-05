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

#include "Mona/Server.h"
#include "Mona/Decoding.h"


using namespace std;



namespace Mona {


ServerManager::ServerManager(Server& server):_server(server),Task(server),Startable("ServerManager"){
}

void ServerManager::run(Exception& ex) {
	do {
		waitHandle();
		_server.relay.manage();
	} while (sleep(2000) != STOP);
}

Server::Server(UInt32 bufferSize,UInt32 threads) : Startable("Server"),Handler(bufferSize,threads),_protocols(*this),_manager(*this) {
#ifndef _WIN32
//	static const char rnd_seed[] = "string to make the random number generator think it has entropy";
//	RAND_seed(rnd_seed, sizeof(rnd_seed));
#endif
}

Server::~Server() {
	stop();
}

bool Server::start(const ServerParams& params) {
	if(running()) {
		ERROR("Server is already running, call stop method before");
		return false;
	}
	(ServerParams&)this->params = params;
	Exception ex;
	bool result;
	EXCEPTION_TO_LOG(result = Startable::start(ex, params.threadPriority), "Server");
	return result;
}

void Server::run(Exception& exc) {
	Exception ex;
	ServerManager manager(*this);
	try {
		TaskHandler::start();

		Exception exWarn;
		if (((SocketManager&)sockets).start(exWarn) && ((RelayServer&)relay).start(exWarn)) {
			if (exWarn)
				WARN(exWarn.error());

			_protocols.load(*this);

			onStart();

			if (!_manager.start(exWarn, Startable::PRIORITY_LOW))
				ex.set(exWarn);
			else if (exWarn)
				WARN(exWarn.error());
			while (!ex && sleep() != STOP)
				giveHandle(ex);
		} else
			ex.set(exWarn);
		if (ex)
			FATAL("Server, %s", ex.error().c_str());
		
	} catch (exception& ex) {
		FATAL("Server, ",ex.what());
	} catch (...) {
		FATAL("Server, unknown error");
	}

	// terminate the tasks
	TaskHandler::stop();
	// terminate messages reception
	((SocketManager&)sockets).stop();
	((RelayServer&)relay).stop();
	// terminate manager
	_manager.stop();

	// clean sessions, and send died message if need
	_sessions.clear();

	// stop receiving and sending engine (it waits the end of sending last session messages)
	poolThreads.clear();

	_protocols.unload();

	onStop();
	NOTE("Server stopped");
}


void Server::readable(Exception& ex,Protocol& protocol) {

	if(!TaskHandler::running()) // If terminad it means that sessions have been deleted: no more reception!
		return;

	SocketAddress address;
	shared_ptr<Buffer<UInt8> > pBuffer;
	if (!protocol.receive(ex, pBuffer, address))
		return;
	if (ex || !pBuffer || !protocol.auth(address))
		return;

	MemoryReader reader(pBuffer->data(),pBuffer->size());

	UInt32 id = protocol.unpack(reader);
	Session* pSession = protocol.session(id,reader);
	if(!pSession) {
		pSession = _sessions.find(id);
		if(!pSession) {
			WARN("Unknown session ",id);
			return;
		}
	}
	shared_ptr<MemoryReader> pReader;
	if (pSession->decode(pBuffer, address, pReader) && pReader) {
		Decoding decoded(id, *this, protocol, pReader, address);
		receive(decoded);
	}
}

void Server::receive(Decoding& decoded) {
	// Process packet
	Session* pSession = decoded.session();
	if(!pSession)
		pSession = _sessions.find(decoded.id);
	if(!pSession)
		return; // No warn because can have been deleted during decoding threading process

	if(decoded.address != pSession->peer.address) {
		SocketAddress oldAddress(pSession->peer.address);
		((SocketAddress&)pSession->peer.address).set(decoded.address);
		pSession->peer.addresses.begin()->set(decoded.address);
		if(pSession->id!=0) // If session is managed by _sessions! (register with Gateway::registerSession)
			_sessions.changeAddress(oldAddress,*pSession);
	}
	
	pSession->receive(decoded.reader());
}

void Server::manage() {
	_protocols.manage();
	_sessions.manage();
}



} // namespace Mona