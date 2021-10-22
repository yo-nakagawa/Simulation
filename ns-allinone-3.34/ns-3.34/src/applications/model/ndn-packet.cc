/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "ndn-packet.h"

namespace ns3 {

NdnPacket::NdnPacket(std::string name, int typeId, std::string content)
{   
    m_name = name;
    m_typeId = typeId;
    m_content = content;
}
NdnPacket::~NdnPacket ()
{
    
}
std::string
NdnPacket::GetName()
{
    return m_name;
}

int
NdnPacket::GetTypeId()
{
    return m_typeId;
}

std::string
NdnPacket::GetContent()
{
    return m_content;
}

} // namespace ns3
