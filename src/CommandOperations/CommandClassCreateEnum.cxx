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

#include <fstream>
#include <ostream>
#include <iostream>

#include "CaretAssertion.h"
#include "CommandClassCreateEnum.h"
#include "FileInformation.h"
#include "ProgramParameters.h"
#include "TextFile.h"

using namespace caret;

/**
 * Constructor.
 */
CommandClassCreateEnum::CommandClassCreateEnum()
: CommandClassCreateBase("-class-create-enum",
                         "CREATE SOURCE CODE CLASS FILES (.h, .cxx) FOR ENUMERATED TYPE")
{
    
}

/**
 * Destructor.
 */
CommandClassCreateEnum::~CommandClassCreateEnum()
{
    
}

/**
 * Execute the operation.
 * 
 * @param parameters
 *   Parameters for the operation.
 * @throws CommandException
 *   If the command failed.
 * @throws ProgramParametersException
 *   If there is an error in the parameters.
 */
void 
CommandClassCreateEnum::executeOperation(ProgramParameters& parameters) throw (CommandException,
                                                               ProgramParametersException)
{
    const AString enumClassName = parameters.nextString("Enum Class Name");
    const int32_t numberOfEnumValues = parameters.nextInt("Number of Enum Values");
    
    if (enumClassName.isEmpty()) {
        throw CommandException("Enum class name is empty.");
    }

    AString errorMessage;
    if (enumClassName.endsWith("Enum") == false) {
        errorMessage += "Name of class MUST end with \"Enum\".\n";
    }
    if (enumClassName[0].isLower()) {
        errorMessage += "First letter of class name must be upper case.\n";
    }

    const AString headerFileName = enumClassName + ".h";
    const AString implementationFileName = enumClassName + ".cxx";
    
    FileInformation headerInfo(headerFileName);
    if (headerInfo.exists()) {
        errorMessage += headerFileName + " exists and this command will not overwrite it.\n";
    }
    FileInformation impInfo(implementationFileName);
    if (impInfo.exists()) {
        errorMessage += implementationFileName + " exists and this command will not overwrite it.\n";
    }
    
    if (errorMessage.isEmpty() == false) {
        throw CommandException(errorMessage);
    }    
    
    AString ifndefName;
    AString ifdefNameStaticDeclarations;
    this->getIfDefNames(enumClassName, 
                        ifndefName, 
                        ifdefNameStaticDeclarations);
    
    this->createHeaderFile(headerFileName,
                           enumClassName, 
                           ifndefName, 
                           ifdefNameStaticDeclarations, 
                           numberOfEnumValues);
    
    this->createImplementationFile(implementationFileName,
                                   enumClassName, 
                                   ifdefNameStaticDeclarations, 
                                   numberOfEnumValues);
}

/**
 * Create and write the header (.h) file.
 *     
 * @param outputFileName
 *    Name for file that is written.
 * @param enumClassName
 *    Name of enumerated type class.
 * @param ifdefName
 *    Name of "ifndef" value.
 * @param ifdefNameStaticDeclaration
 *    Name for "infdef" of static declarations.
 * @param numberOfEnumValues
 *    Number of enumerated type values.
 */
void 
CommandClassCreateEnum::createHeaderFile(const AString& outputFileName,
                                         const AString& enumClassName,
                                         const AString& ifndefName,
                                         const AString& ifdefNameStaticDeclaration,
                                         const int32_t numberOfEnumValues)
{
    AString t;

    t += ("#ifndef " + ifndefName + "\n");
    t += ("#define " + ifndefName + "\n");
    t += this->getCopyright();
    t += ("\n");
    
    t += ("#include <stdint.h>\n");
    t += ("#include <vector>\n");
    t += ("#include \"AString.h\"\n");
    t += ("\n");
    t += ("namespace caret {\n");
    t += ("\n");
    t += ("/// Enumerated type for \n");
    t += ("class " + enumClassName + " {\n");
    t += ("\n");
    t += ("public:\n");
    t += ("    /**\n");
    t += ("     * Enumerated values.\n");
    t += ("     */\n");
    t += ("    enum Enum {\n");
    
    for (int indx = 0; indx < numberOfEnumValues; indx++) {
        t += ("        /** */\n");
        if (indx < (numberOfEnumValues - 1)) {
            t += ("        ,\n");
        }
        else {
            t += ("        \n");
        }
    }
    
    t += ("    };\n");
    t += ("\n");
    t += ("\n");
    t += ("    ~" + enumClassName + "();\n");
    t += ("\n");
    t += ("    static AString toName(Enum enumValue);\n");
    t += ("    \n");
    t += ("    static Enum fromName(const AString& name, bool* isValidOut);\n");
    t += ("    \n");
    t += ("    static AString toGuiName(Enum enumValue);\n");
    t += ("    \n");
    t += ("    static Enum fromGuiName(const AString& guiName, bool* isValidOut);\n");
    t += ("    \n");
    t += ("    static int32_t toIntegerCode(Enum enumValue);\n");
    t += ("    \n");
    t += ("    static Enum fromIntegerCode(const int32_t integerCode, bool* isValidOut);\n");
    t += ("\n");
    t += ("    static void getAllEnums(std::vector<Enum>& allEnums);\n");
    t += ("\n");
    t += ("private:\n");
    t += ("    " + enumClassName + "(const Enum enumValue, \n");
    t += ("                 const int32_t integerCode, \n");
    t += ("                 const AString& name,\n");
    t += ("                 const AString& guiName);\n");
    t += ("\n");
    t += ("    static const " + enumClassName + "* findData(const Enum enumValue);\n");
    t += ("\n");
    t += ("    /** Holds all instance of enum values and associated metadata */\n");
    t += ("    static std::vector<" + enumClassName + "> enumData;\n");
    t += ("\n");
    t += ("    /** Initialize instances that contain the enum values and metadata */\n");
    t += ("    static void initialize();\n");
    t += ("\n");
    t += ("    /** Indicates instance of enum values and metadata have been initialized */\n");
    t += ("    static bool initializedFlag;\n");
    t += ("    \n");
    t += ("    /** The enumerated type value for an instance */\n");
    t += ("    Enum enumValue;\n");
    t += ("\n");
    t += ("    /** The integer code associated with an enumerated value */\n");
    t += ("    int32_t integerCode;\n");
    t += ("\n");
    t += ("    /** The name, a text string that is identical to the enumerated value */\n");
    t += ("    AString name;\n");
    t += ("    \n");
    t += ("    /** A user-friendly name that is displayed in the GUI */\n");
    t += ("    AString guiName;\n");
    t += ("};\n");
    t += ("\n");
    t += ("#ifdef " + ifdefNameStaticDeclaration + "\n");
    t += ("std::vector<" + enumClassName + "> " + enumClassName + "::enumData;\n");
    t += ("bool " + enumClassName + "::initializedFlag = false;\n");
    t += ("#endif // " + ifdefNameStaticDeclaration + "\n");
    t += ("\n");
    t += ("} // namespace\n");
    
    t += ("#endif  //" + ifndefName + "\n");

    TextFile tf;
    tf.replaceText(t);
    
    try {
        tf.writeFile(outputFileName);
    }
    catch (DataFileException e) {
        throw CommandException(e);
    }
}

/**
 * Create and write the implementation (.cxx) file.
 *     
 * @param outputFileName
 *    Name for file that is written.
 * @param enumClassName
 *    Name of enumerated type class.
 * @param ifdefNameStaticDeclaration
 *    Name for "infdef" of static declarations.
 * @param numberOfEnumValues
 *    Number of enumerated type values.
 */
void 
CommandClassCreateEnum::createImplementationFile(const AString& outputFileName,
                                                 const AString& enumClassName,
                                                 const AString& ifdefNameStaticDeclaration,
                                                 const int32_t numberOfEnumValues)
{
    AString t;
    
    t += this->getCopyright();
    
    t += ("#define " + ifdefNameStaticDeclaration + "\n");
    t += ("#include \"" + enumClassName + ".h\"\n");
    t += ("#undef " + ifdefNameStaticDeclaration + "\n");
    t += ("\n");
    t += ("#include \"CaretAssert.h\"\n");
    t += ("\n");
    t += ("using namespace caret;\n");
    t += ("\n");
    t += ("/**\n");
    t += (" * Constructor.\n");
    t += (" *\n");
    t += (" * @param enumValue\n");
    t += (" *    An enumerated value.\n");
    t += (" * @param integerCode\n");
    t += (" *    Integer code for this enumerated value.\n");
    t += (" *\n");
    t += (" * @param name\n");
    t += (" *    Name of enumerated value.\n");
    t += (" */\n");
    t += (" * @param guiName\n");
    t += (" *    User-friendly name for use in user-interface.\n");
    t += (" *\n");
    t += ("" + enumClassName + "::" + enumClassName + "(const Enum enumValue,\n");
    t += ("                           const int32_t integerCode,\n");
    t += ("                           const AString& name,\n");
    t += ("                           const AString& guiName)\n");
    t += ("{\n");
    t += ("    this->enumValue = enumValue;\n");
    t += ("    this->integerCode = integerCode;\n");
    t += ("    this->name = name;\n");
    t += ("    this->guiName = guiName;\n");
    t += ("}\n");
    t += ("\n");
    
    
    t += ("/**\n");
    t += (" * Destructor.\n");
    t += (" */\n");
    t += ("" + enumClassName + "::~" + enumClassName + "()\n");
    t += ("{\n");
    t += ("}\n");
    t += ("\n");
    
    
    t += ("/**\n");
    t += (" * Initialize the enumerated metadata.\n");
    t += (" */\n");
    t += ("void\n");
    t += ("" + enumClassName + "::initialize()\n");
    t += ("{\n");
    t += ("    if (initializedFlag) {\n");
    t += ("        return;\n");
    t += ("    }\n");
    t += ("    initializedFlag = true;\n");
    t += ("\n");
    
    for (int32_t indx = 0; indx < numberOfEnumValues; indx++) {
        t += ("    enumData.push_back(" + enumClassName + "(, \n");
        t += ("                                    " + AString::number(indx) + ", \n");
        t += ("                                    \"\", \n");
        t += ("                                    \"\"));\n");
        t += ("    \n");
    }
    t += ("}\n");
    t += ("\n");
    
    
    t += ("/**\n");
    t += (" * Find the data for and enumerated value.\n");
    t += (" * @param enumValue\n");
    t += (" *     The enumerated value.\n");
    t += (" * @return Pointer to data for this enumerated type\n");
    t += (" * or NULL if no data for type or if type is invalid.\n");
    t += (" */\n");
    t += ("const " + enumClassName + "*\n");
    t += ("" + enumClassName + "::findData(const Enum enumValue)\n");
    t += ("{\n");
    t += ("    if (initializedFlag == false) initialize();\n");
    t += ("\n");
    t += ("    size_t num = enumData.size();\n");
    t += ("    for (size_t i = 0; i < num; i++) {\n");
    t += ("        const " + enumClassName + "* d = &enumData[i];\n");
    t += ("        if (d->enumValue == enumValue) {\n");
    t += ("            return d;\n");
    t += ("        }\n");
    t += ("    }\n");
    t += ("\n");
    t += ("    return NULL;\n");
    t += ("}\n");
    t += ("\n");
    
    
    t += ("/**\n");
    t += (" * Get a string representation of the enumerated type.\n");
    t += (" * @param enumValue \n");
    t += (" *     Enumerated value.\n");
    t += (" * @return \n");
    t += (" *     String representing enumerated value.\n");
    t += (" */\n");
    t += ("AString \n");
    t += ("" + enumClassName + "::toName(Enum enumValue) {\n");
    t += ("    if (initializedFlag == false) initialize();\n");
    t += ("    \n");
    t += ("    const " + enumClassName + "* enumInstance = findData(enumValue);\n");
    t += ("    return enumInstance->name;\n");
    t += ("}\n");
    t += ("\n");
    
    
    t += ("/**\n");
    t += (" * Get an enumerated value corresponding to its name.\n");
    t += (" * @param name \n");
    t += (" *     Name of enumerated value.\n");
    t += (" * @param isValidOut \n");
    t += (" *     If not NULL, it is set indicating that a\n");
    t += (" *     enum value exists for the input name.\n");
    t += (" * @return \n");
    t += (" *     Enumerated value.\n");
    t += (" */\n");
    t += ("" + enumClassName + "::Enum \n");
    t += ("" + enumClassName + "::fromName(const AString& name, bool* isValidOut)\n");
    t += ("{\n");
    t += ("    if (initializedFlag == false) initialize();\n");
    t += ("    \n");
    t += ("    bool validFlag = false;\n");
    t += ("    Enum enumValue;\n");
    t += ("    \n");
    t += ("    for (std::vector<" + enumClassName + ">::iterator iter = enumData.begin();\n");
    t += ("         iter != enumData.end();\n");
    t += ("         iter++) {\n");
    t += ("        const " + enumClassName + "& d = *iter;\n");
    t += ("        if (d.name == name) {\n");
    t += ("            enumValue = d.enumValue;\n");
    t += ("            validFlag = true;\n");
    t += ("            break;\n");
    t += ("        }\n");
    t += ("    }\n");
    t += ("    \n");
    t += ("    if (isValidOut != 0) {\n");
    t += ("        *isValidOut = validFlag;\n");
    t += ("    }\n");
    t += ("    else {\n");
    t += ("        CaretAssertMessage(0, AString(\"Name \" + name + \"failed to match enumerated value for type " + enumClassName + "\"));\n");
    t += ("    }\n");
    t += ("    return enumValue;\n");
    t += ("}\n");
    t += ("\n");
    
    
    t += ("/**\n");
    t += (" * Get a GUI string representation of the enumerated type.\n");
    t += (" * @param enumValue \n");
    t += (" *     Enumerated value.\n");
    t += (" * @return \n");
    t += (" *     String representing enumerated value.\n");
    t += (" */\n");
    t += ("AString \n");
    t += ("" + enumClassName + "::toGuiName(Enum enumValue) {\n");
    t += ("    if (initializedFlag == false) initialize();\n");
    t += ("    \n");
    t += ("    const " + enumClassName + "* enumInstance = findData(enumValue);\n");
    t += ("    return enumInstance->guiName;\n");
    t += ("}\n");
    t += ("\n");
    
    
    t += ("/**\n");
    t += (" * Get an enumerated value corresponding to its GUI name.\n");
    t += (" * @param s \n");
    t += (" *     Name of enumerated value.\n");
    t += (" * @param isValidOut \n");
    t += (" *     If not NULL, it is set indicating that a\n");
    t += (" *     enum value exists for the input name.\n");
    t += (" * @return \n");
    t += (" *     Enumerated value.\n");
    t += (" */\n");
    t += ("" + enumClassName + "::Enum \n");
    t += ("" + enumClassName + "::fromGuiName(const AString& guiName, bool* isValidOut)\n");
    t += ("{\n");
    t += ("    if (initializedFlag == false) initialize();\n");
    t += ("    \n");
    t += ("    bool validFlag = false;\n");
    t += ("    Enum enumValue;\n");
    t += ("    \n");
    t += ("    for (std::vector<" + enumClassName + ">::iterator iter = enumData.begin();\n");
    t += ("         iter != enumData.end();\n");
    t += ("         iter++) {\n");
    t += ("        const " + enumClassName + "& d = *iter;\n");
    t += ("        if (d.guiName == guiName) {\n");
    t += ("            enumValue = d.enumValue;\n");
    t += ("            validFlag = true;\n");
    t += ("            break;\n");
    t += ("        }\n");
    t += ("    }\n");
    t += ("    \n");
    t += ("    if (isValidOut != 0) {\n");
    t += ("        *isValidOut = validFlag;\n");
    t += ("    }\n");
    t += ("    else {\n");
    t += ("        CaretAssertMessage(0, AString(\"guiName \" + guiName + \"failed to match enumerated value for type " + enumClassName + "\"));\n");
    t += ("    }\n");
    t += ("    return enumValue;\n");
    t += ("}\n");
    t += ("\n");
    
    
    t += ("/**\n");
    t += (" * Get the integer code for a data type.\n");
    t += (" *\n");
    t += (" * @return\n");
    t += (" *    Integer code for data type.\n");
    t += (" */\n");
    t += ("int32_t\n");
    t += ("" + enumClassName + "::toIntegerCode(Enum enumValue)\n");
    t += ("{\n");
    t += ("    if (initializedFlag == false) initialize();\n");
    t += ("    const " + enumClassName + "* enumInstance = findData(enumValue);\n");
    t += ("    return enumInstance->integerCode;\n");
    t += ("}\n");
    t += ("\n");
    
    
    t += ("/**\n");
    t += (" * Find the data type corresponding to an integer code.\n");
    t += (" *\n"); 
    t += (" * @param integerCode\n");
    t += (" *     Integer code for enum.\n");
    t += (" * @param isValidOut\n");
    t += (" *     If not NULL, on exit isValidOut will indicate if\n");
    t += (" *     integer code is valid.\n");
    t += (" * @return\n");
    t += (" *     Enum for integer code.\n");
    t += (" */\n");
    t += ("" + enumClassName + "::Enum\n");
    t += ("" + enumClassName + "::fromIntegerCode(const int32_t integerCode, bool* isValidOut)\n");
    t += ("{\n");
    t += ("    if (initializedFlag == false) initialize();\n");
    t += ("    \n");
    t += ("    bool validFlag = false;\n");
    t += ("    Enum enumValue = <REPLACE_WITH_DEFAULT_ENUM_VALUE>;\n");
    t += ("    \n");
    t += ("    for (std::vector<" + enumClassName + ">::iterator iter = enumData.begin();\n");
    t += ("         iter != enumData.end();\n");
    t += ("         iter++) {\n");
    t += ("        const " + enumClassName + "& enumInstance = *iter;\n");
    t += ("        if (enumInstance.integerCode == integerCode) {\n");
    t += ("            enumValue = enumInstance.enumValue;\n");
    t += ("            validFlag = true;\n");
    t += ("            break;\n");
    t += ("        }\n");
    t += ("    }\n");
    t += ("    \n");
    t += ("    if (isValidOut != 0) {\n");
    t += ("        *isValidOut = validFlag;\n");
    t += ("    }\n");
    t += ("    else {\n");
    t += ("        CaretAssertMessage(0, AString(\"Integer code \" + AString::number(integerCode) + \"failed to match enumerated value for type " + enumClassName + "\"));\n");
    t += ("    }\n");
    t += ("    return enumValue;\n");
    t += ("}\n");
    t += ("\n");
    
    
    t += ("/**\n");
    t += (" * Get all of the enumerated type values.  The values can be used\n");
    t += (" * as parameters to toXXX() methods to get associated metadata.\n"); 
    t += (" *\n");
    t += (" * @param allEnums\n");
    t += (" *     A vector that is OUTPUT containing all of the enumerated values.\n");
    t += (" */\n");
    t += ("void\n");
    t += ("" + enumClassName + "::getAllEnums(std::vector<" + enumClassName + "::Enum>& allEnums)\n");
    t += ("{\n");
    t += ("    if (initializedFlag == false) initialize();\n");
    t += ("    \n");
    t += ("    allEnums.clear();\n");
    t += ("    \n");
    t += ("    for (std::vector<" + enumClassName + ">::iterator iter = enumData.begin();\n");
    t += ("         iter != enumData.end();\n");
    t += ("         iter++) {\n");
    t += ("        allEnums.push_back(iter->enumValue);\n");
    t += ("    }\n");
    t += ("}\n");
    t += ("\n");

    
    TextFile tf;
    tf.replaceText(t);
    
    try {
        tf.writeFile(outputFileName);
    }
    catch (DataFileException e) {
        throw CommandException(e);
    }
}

