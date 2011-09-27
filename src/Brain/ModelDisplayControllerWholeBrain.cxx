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

#include "ModelDisplayControllerWholeBrain.h"

using namespace caret;

/**
 * Constructor.
 * @param brain - brain to which this surface controller belongs.
 * @param surface - surface for this controller.
 *
 */
ModelDisplayControllerWholeBrain::ModelDisplayControllerWholeBrain(Brain* brain)
: ModelDisplayController(ModelDisplayControllerTypeEnum::MODEL_TYPE_WHOLE_BRAIN,
                         YOKING_ALLOWED_NO,
                         ROTATION_ALLOWED_YES)
{
    this->initializeMembersModelDisplayControllerWholeBrain();
}

/**
 * Destructor
 */
ModelDisplayControllerWholeBrain::~ModelDisplayControllerWholeBrain()
{
}

void
ModelDisplayControllerWholeBrain::initializeMembersModelDisplayControllerWholeBrain()
{
}

/**
 * Get the brain that created this controller.
 * @return The brain.
 */
Brain*
ModelDisplayControllerWholeBrain::getBrain()
{
    return this->brain;
}

/**
 * Get the name for use in a GUI.
 *
 * @param includeStructureFlag - Prefix label with structure to which
 *      this structure model belongs.
 * @return   Name for use in a GUI.
 *
 */
AString
ModelDisplayControllerWholeBrain::getNameForGUI(const bool includeStructureFlag) const
{
    return "ModelDisplayControllerWholeBrain::getNameForGUI_NEEDS_IMPLEMENTATION";
}

