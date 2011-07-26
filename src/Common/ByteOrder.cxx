/*LICENSE_START*/ 
/* 
 *  Copyright 1995-2002 Washington University School of Medicine 
 * 
 *  http://brainmap.wustl.edu 
 * 
 *  This file is part of CARET. 
 * 
 *  CARET is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation; either version 2 of the License, or 
 *  (at your option) any later version. 
 * 
 *  CARET is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 * 
 *  You should have received a copy of the GNU General Public License 
 *  along with CARET; if not, write to the Free Software 
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 */ 


#define __BYTE_ORDER_DECLARE__
#include "ByteOrder.h"
#undef __BYTE_ORDER_DECLARE__


using namespace caret;

/**
 * Constructor.
 *
 * @param e
 *    An enumerated value.
 * @param name
 *    Name of enumberated value.
 */
ByteOrder::ByteOrder(
                   const Enum e,
                   const std::string& name)
{
    this->e = e;
    this->name = name;
}

/**
 * Destructor.
 */
ByteOrder::~ByteOrder()
{
}

/**
 * Get the system byte order.
 * @return
 *    Byte order of the system.
 */

ByteOrder::Enum 
ByteOrder::getSystemEndian()
{
    ByteOrder::initialize();
    return ByteOrder::systemEndian;
}

/**
 * Is the system byte order little endian?
 * @return
 *    true if system is little endian byte order.
 */
bool 
ByteOrder::isSystemLittleEndian()
{
    ByteOrder::initialize();
    return (ByteOrder::systemEndian == ENDIAN_LITTLE);    
}

/**
 * Is the system byte order big endian?
 * @return
 *    true if system is big endian byte order.
 */
bool 
ByteOrder::isSystemBigEndian()
{
    ByteOrder::initialize();
    return (ByteOrder::systemEndian == ENDIAN_BIG);    
}

void
ByteOrder::initialize()
{
    if (initializedFlag) {
        return;
    }
    initializedFlag = true;

    enumData.push_back(ByteOrder(ENDIAN_BIG,"ENDIAN_BIG"));
    enumData.push_back(ByteOrder(ENDIAN_LITTLE,"ENDIAN_LITTLE"));
    
    uint32_t intVal = 0x00000001;
    unsigned char* c = (unsigned char*)&intVal;
    
    ByteOrder::systemEndian = ByteOrder::ENDIAN_BIG;
    if (*c == 0x01) systemEndian = ByteOrder::ENDIAN_LITTLE;
}

/**
 * Get the enum value for this enumerated item.
 * @return the value for this enumerated item.
 */
ByteOrder::Enum 
ByteOrder::getEnum() const
{
    return this->e;
}

/**
 * Get the enum name for this enumerated item.
 * @return the name for this enumerated item.
 */
std::string
ByteOrder::getName() const
{
    return this->name;
}

/**
 * Find the data for and enumerated value.
 * @param e
 *     The enumerated value.
 * @return Pointer to data for this enumerated type
 * or NULL if no data for type or if type is invalid.
 */
const ByteOrder*
ByteOrder::findData(const Enum e)
{
    initialize();
    int64_t num = enumData.size();
    for (int64_t i = 0; i < num; i++) {
        const ByteOrder* d = &enumData[i];
        if (d->e == e) {
            return d;
        }
    }

    return NULL;
}

/**
 * Get a string representation of the enumerated type.
 * @param e 
 *     Enumerated value.
 * @param isValidOut 
 *     If not NULL, it is set indicating that a
 *     label exists for the input enum value.
 * @return 
 *     String representing enumerated value.
 */
std::string 
ByteOrder::toString(Enum e, bool* isValidOut) {
    initialize();
    
    std::string s;
    
    for (std::vector<ByteOrder>::iterator iter = enumData.begin();
         iter != enumData.end();
         iter++) {
        const ByteOrder& d = *iter;
        if (d.e == e) {
            s = d.name;
            break;
        }
    }

    if (isValidOut != NULL) {
        *isValidOut = (s.size() > 0);
    }
    
    return s;
}

/**
 * Get an enumerated value corresponding to its name.
 * @param s 
 *     Name of enumerated value.
 * @param isValidOut 
 *     If not NULL, it is set indicating that a
 *     enum value exists for the input name.
 * @return 
 *     Enumerated value.
 */
ByteOrder::Enum 
ByteOrder::fromString(const std::string& s, bool* isValidOut)
{
    initialize();
    
    bool validFlag = false;
    Enum e;
    
    for (std::vector<ByteOrder>::iterator iter = enumData.begin();
         iter != enumData.end();
         iter++) {
        const ByteOrder& d = *iter;
        if (d.name == s) {
            e = d.e;
            validFlag = true;
            break;
        }
    }
    
    if (isValidOut != 0) {
        *isValidOut = validFlag;
    }
    return e;
}
