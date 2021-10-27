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

#ifndef NDN_PACKET_H
#define NDN_PACKET_H

#include "ns3/application.h"
#include "/home/nuc1/Simulation/cereal-1.3.0/include/cereal/cereal.hpp"

namespace ns3 {
class NdnPacket 
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  NdnPacket (std::string name, int typeId, std::string content);
  ~NdnPacket ();
  std::string GetName();
  int GetTypeId();
  std::string GetContent();
  template<class Archive>
  void serialize(Archive & archive)
  {
    archive(CEREAL_NVP(m_name), CEREAL_NVP(m_typeId), CEREAL_NVP(m_content) );
  }
private:
    std::string m_name;
    int m_typeId;             //0:Interest, 1:Data
    std::string m_content;
};

} // namespace ns3

#endif /* NDN_PACKET_H */