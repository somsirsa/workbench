#ifndef __NIFTISPACINGUNITS_H__
#define __NIFTISPACINGUNITS_H__

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


#include <stdint.h>

#include <string>
#include <vector>

namespace caret {

/**
 * NIFTI Spacing Units
 */
class NiftiSpacingUnits {

public:
    /**  NIFTI Spacing Units
 */
    enum Enum {
        /**  */
        NIFTI_UNITS_UNKNOWN,
        /**  */
        NIFTI_UNITS_METER,
        /**  */
        NIFTI_UNITS_MM,
        /**  */
        NIFTI_UNITS_MICRON
    };


    ~NiftiSpacingUnits();
private:
    NiftiSpacingUnits(Enum e, const int32_t integerCode, const std::string& name);
    static std::vector<NiftiSpacingUnits> spacingUnits;
    static void initialize();
    static bool initializedFlag;
    
    Enum e;
    int32_t integerCode;
    std::string name;
public:
    static std::string toString(Enum e, bool* isValidOut);
    static Enum fromString(const std::string& s, bool* isValidOut);
    
};
  
#ifdef __NIFTI_SPACING_UNITS_DECLARE__
    std::vector<NiftiSpacingUnits> NiftiSpacingUnits::spacingUnits;
    bool NiftiSpacingUnits::initializedFlag = false;
#endif // __NIFTI_SPACING_UNITS_DECLARE__

} // namespace

#endif // __NIFTISPACINGUNITS_H__
