#ifndef __PALETTETHRESHOLDTEST_H__
#define __PALETTETHRESHOLDTEST_H__

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
#include <vector>
#include <string>

namespace caret {

/**
 * Palette Threshold Test.
 */
class PaletteThresholdTest {

public:
    /** Palette Threshold Test. */
    enum Enum {
        /** show data when value is greater than threshold value */
        THRESHOLD_TEST_SHOW_ABOVE,
        /** show data when value is less than threshold value */
        THRESHOLD_TEST_SHOW_BELOW
    };


    ~PaletteThresholdTest();

    Enum getEnum() const;

    int32_t getIntegerCode() const;

    std::string getName() const;

    std::string getGuiName() const;
    
    static std::string toString(Enum e, bool* isValidOut);
    
    static Enum fromString(const std::string& s, bool* isValidOut);
    
    static const PaletteThresholdTest* findData(const Enum e);
private:
    PaletteThresholdTest(const Enum e, const int32_t integerCode, const std::string& name, const std::string& guiName);

    static std::vector<PaletteThresholdTest> enumData;

    static void initialize();

    static bool initializedFlag;

    Enum e;

    int32_t integerCode;

    std::string name;
    
    std::string guiName;


};

#ifdef __PALETTE_THRESHOLD_TEST_DECLARE__
    std::vector<PaletteThresholdTest> PaletteThresholdTest::enumData;
bool PaletteThresholdTest::initializedFlag = false;
#endif // __PALETTE_THRESHOLD_TEST_DECLARE__

} // namespace

#endif // __PALETTETHRESHOLDTEST_H__
