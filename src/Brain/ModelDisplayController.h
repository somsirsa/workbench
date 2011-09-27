#ifndef __MODEL_DISPLAY_CONTROLLER_H__
#define __MODEL_DISPLAY_CONTROLLER_H__

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

#include "BrainConstants.h"
#include "CaretObject.h"
#include "ModelDisplayControllerTypeEnum.h"

#include "Matrix4x4.h"

namespace caret {

    /// Base class for controlling a model
    class ModelDisplayController : public CaretObject {
        
    protected:
        enum YokingAllowedType {
            YOKING_ALLOWED_YES,
            YOKING_ALLOWED_NO
        };
        
        enum RotationAllowedType {
            ROTATION_ALLOWED_YES,
            ROTATION_ALLOWED_NO
        };
        
        ModelDisplayController(const ModelDisplayControllerTypeEnum::Enum controllerType,
                               const YokingAllowedType allowsYokingStatus,
                               const RotationAllowedType allowsRotationStatus);
        
        virtual ~ModelDisplayController();
        
        
    private:        
        ModelDisplayController(const ModelDisplayController& o);
        ModelDisplayController& operator=(const ModelDisplayController& o);
        
        void initializeMembersModelDisplayController();
        
    public:
        ModelDisplayControllerTypeEnum::Enum getControllerType() const;
        
        virtual AString getNameForGUI(const bool includeStructureFlag) const = 0;
        
        bool isRotationAllowed() const;
        
        bool isYokeable() const;
        
        void copyTransformations(const ModelDisplayController& controller,
                                 const int32_t windowTabNumberSource,
                                 const int32_t windowTabNumberTarget);
        
        Matrix4x4* getViewingRotationMatrix(const int32_t windowTabNumber);
        
        const Matrix4x4* getViewingRotationMatrix(const int32_t windowTabNumber) const;
        
        const float* getTranslation(const int32_t windowTabNumber) const;
        
        void setTranslation(const int32_t windowTabNumber,
                            const float t[3]);
        
        void setTranslation(const int32_t windowTabNumber,
                            const float tx,
                            const float ty,
                            const float tz);
        
        float getScaling(const int32_t windowTabNumber) const;
        
        void setScaling(const int32_t windowTabNumber,
                        const float s);
        
        virtual void resetView(const int32_t windowTabNumber);
        
        virtual void rightView(const int32_t windowTabNumber);
        
        virtual void leftView(const int32_t windowTabNumber);
        
        virtual void anteriorView(const int32_t windowTabNumber);
        
        virtual void posteriorView(const int32_t windowTabNumber);
        
        virtual void dorsalView(const int32_t windowTabNumber);
        
        virtual void ventralView(const int32_t windowTabNumber);
        
        void setTransformation(const int32_t windowTabNumber,
                               const std::vector<float>& transformationData);
        
        virtual AString toString() const;
        
        virtual AString toDescriptiveString() const;
        
    private:
        void resetViewPrivate(const int windowTabNumber);
        
    protected:
        float defaultModelScaling;
        
        /**the viewing rotation matrix. */
        Matrix4x4 viewingRotationMatrix[BrainConstants::MAXIMUM_NUMBER_OF_BROWSER_TABS];
        
        /**translation. */
        float translation[BrainConstants::MAXIMUM_NUMBER_OF_BROWSER_TABS][3];
        
        /**scaling. */
        float scaling[BrainConstants::MAXIMUM_NUMBER_OF_BROWSER_TABS];
        
    private:
        ModelDisplayControllerTypeEnum::Enum controllerType;
        
        YokingAllowedType allowsYokingStatus;
        
        RotationAllowedType allowsRotationStatus;
        
    };

} // namespace

#endif // __MODEL_DISPLAY_CONTROLLER_H__
