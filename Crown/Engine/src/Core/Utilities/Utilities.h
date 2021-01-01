#pragma once

#include "Core/Utilities/Defines.h"

#include <string>
//#include <glm/glm.hpp>

namespace Crown
{
    /// String hash using sdbm algorithm
    ullong StringHash(const std::string& a_String);
    std::string PointerToString(const void* a_Pointer);
    bool Replace(std::string& a_Str, const std::string& a_From, const std::string& a_To);
    //glm::vec3 Refract(glm::vec3 a_Vector, glm::vec3 a_Normal, float a_Eta);
    float Random();
    float Random(float a_Min, float a_Max);
    int Random(int a_Min, int a_Max);
    //glm::vec3 RandomDirectionInHemisphere(const glm::vec3& a_Normal);

}