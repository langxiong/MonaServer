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

#include "Mona/BlindlyProxy/Proxy.h"
#include "Mona/Crypto.h"

using namespace std;
namespace Mona {

bool Proxy::Unpack(BinaryReader& reader) {
	reader.reset();
    return false;
}

void Proxy::Pack(BinaryWriter& writer) {
	/*BinaryReader reader(writer.data()+4,writer.size()-4);
	BinaryWriter(writer.data(),4).write32(reader.read32()^reader.read32()^farId);*/
}

}  // namespace Mona
