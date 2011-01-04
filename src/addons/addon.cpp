//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2010 Lucas Baudin, Joerg Henrichs
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
/**
  \page addons Addons
  */
#ifdef ADDONS_MANAGER

#include "addons/addon.hpp"

#include <fstream>
//#include <iostream>

#include "io/file_manager.hpp"
#include "io/xml_node.hpp"
#include "utils/string_utils.hpp"

Addon::Addon(const XMLNode &xml, bool installed)
{
    m_installed         = installed;
    m_installed_version = 0;
    m_name              = "";
    m_version           = 0 ;
    m_zip_file          = "";
    m_description       = "";
    m_icon              = "";
    m_id                = "";
    m_type              = xml.getName();
    
    xml.get("name",        &m_name);
    if(m_installed)
    {
        xml.get("installed-version", &m_installed_version);
        xml.get("id",      &m_id             );
    }
    else   // not installed
    {
        xml.get("file",        &m_zip_file   );
        xml.get("description", &m_description);
        xml.get("icon",        &m_icon       );
        xml.get("version",     &m_version    );
        // The online list has a numeric id, which is not used.
        // So ignore it.
        m_id = StringUtils::toLowerCase(m_name);
    }   // if installed
};   // Addon(const XML&)

// ----------------------------------------------------------------------------
/** Copies the installation data (like description, version, icon) from the 
 *  downloaded online list to this entry.
*/
void Addon::copyInstallData(const Addon &addon)
{
    m_description = addon.m_description;
    m_version     = addon.m_version;
    m_zip_file    = addon.m_zip_file;
    m_icon        = addon.m_icon;
}   // copyInstallData

// ----------------------------------------------------------------------------
/** Writes information about an installed addon (it is only called for 
 *  installed addons).
 *  \param out_stream Output stream to write to.
 */
void Addon::writeXML(std::ofstream *out_stream)
{
    (*out_stream) << "  <" << m_type << " name=\"" << m_name 
                  << "\" id=\"" << m_id << "\" installed-version=\""
                  << m_installed_version << "\"/>\n";
}   // writeXML

#endif

