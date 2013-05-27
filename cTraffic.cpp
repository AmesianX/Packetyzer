/*
 *
 *  Copyright (C) 2013  Anwar Mohamed <anwarelmakrahy[at]gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to Anwar Mohamed
 *  anwarelmakrahy[at]gmail.com
 *
 */

#include "Packetyzer.h"

using namespace std;
using namespace Packetyzer::Traffic::Connections;
using namespace Packetyzer::Traffic::Streams;

cTraffic::cTraffic()
{
	nConnections = 0;
	Connections = (cConnection**)malloc( sizeof(cConnection*) * nConnections);
}

BOOL cTraffic::AddPacket(cPacket* Packet, UINT TimeStamp)
{
	cConnection* TmpConnection = NULL;

	if (nConnections > 0)
	{
		for (UINT j=0; j<nConnections; j++)
		{
			if (Connections[j]->AddPacket(Packet)) return TRUE;
				
			if (j == (nConnections - 1))
			{

				if (cConStream::Identify(Packet))
				{
					/* TCP Application Layers */
					if (cTCPStream::Identify(Packet))
					{
						if (cHTTPStream::Identify(Packet))
							TmpConnection = new cHTTPStream();
						else
							TmpConnection = new cTCPStream();	 
					}

					/* UDP Application Layers */
					else if (cUDPStream::Identify(Packet))
					{
						if (cDNSStream::Identify(Packet))
							TmpConnection = new cDNSStream();	 
						else 
							TmpConnection = new cUDPStream();
					}
				}
				else if (cICMPStream::Identify(Packet))
					TmpConnection = new cICMPStream();	
				else if (cARPStream::Identify(Packet))
					TmpConnection = new cARPStream();
				else 
					TmpConnection = new cConnection();	
			}
		}
	}
	else
	{
		if (cConStream::Identify(Packet))
		{
			/* TCP Application Layers */
			if (cTCPStream::Identify(Packet))
			{
				if (cHTTPStream::Identify(Packet))
					TmpConnection = new cHTTPStream();
				else
					TmpConnection = new cTCPStream();	 
			}

			/* UDP Application Layers */
			else if (cUDPStream::Identify(Packet))
			{

				if (cDNSStream::Identify(Packet))
					TmpConnection = new cDNSStream();	 
				else 
					TmpConnection = new cUDPStream();

			}

		}
		
		else if (cICMPStream::Identify(Packet))
			TmpConnection = new cICMPStream();
		else if (cARPStream::Identify(Packet))
			TmpConnection = new cARPStream();
		else 
			TmpConnection = new cConnection();	

	}

	TmpConnection->AddPacket(Packet);	
	nConnections++;
	Connections = (cConnection**)realloc((void*)Connections, nConnections * sizeof(cConnection*));
	memcpy((void**)&Connections[nConnections-1],(void**)&TmpConnection, sizeof(cConnection*));
	return TRUE;
}

cTraffic::~cTraffic()
{
}
