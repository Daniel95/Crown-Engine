#include "CrownPCH.h"

#include "Utilities.h"

namespace Crown
{
    // String hashing function
    // Taken from http://www.cse.yorku.ca/~oz/hash.html (public domain)
    ullong StringHash(const std::string& a_String)
    {
        CROWN_PROFILE_FUNCTION();

        ullong hash = 0;
        int c;
        const char* cstr = a_String.c_str();

        while ((c = *cstr++))
        {
            hash = c + (hash << 6) + (hash << 16) - hash;
        }

        return hash;
    }

    std::string PointerToString(const void* a_Pointer)
    {
        CROWN_PROFILE_FUNCTION();

        const void* address = static_cast<const void*>(a_Pointer);
        std::stringstream ss;
        ss << address;
        return ss.str();
    }

    bool Replace(std::string& a_Str, const std::string& a_From, const std::string& a_To)
    {
        CROWN_PROFILE_FUNCTION();

        size_t start_pos = a_Str.find(a_From);
        if (start_pos == std::string::npos)
        {
            return false;
        }
        a_Str.replace(start_pos, a_From.length(), a_To);
        return true;
    }

    /*
    glm::vec3 Refract(glm::vec3 a_Vector, glm::vec3 a_Normal, float a_Eta)
    {
        float k = 1.0f - a_Eta * a_Eta * (1.0f - glm::dot(a_Normal, a_Vector) * glm::dot(a_Normal, a_Vector));

        if (k < 0.0f)
        {
            return glm::vec3(0);
        }
        else
        {
            return a_Eta * a_Vector - (a_Eta * glm::dot(a_Normal, a_Vector) + sqrt(k)) * a_Normal;
        }
    }
    */

    float Random()
    {
        CROWN_PROFILE_FUNCTION();

        return static_cast<float>(rand()) / RAND_MAX;
    }

    /*
    glm::vec3 RandomDirectionInHemisphere(const glm::vec3& a_Normal)
    {
        glm::vec3 hemisphere;

        float azimuth = Random() * glm::pi<float>() * 2.0f;
        hemisphere.y = Random();

        float sin_elevation = sqrtf(1 - hemisphere.y * hemisphere.y);
        hemisphere.x = sin_elevation * cos(azimuth);
        hemisphere.z = sin_elevation * sin(azimuth);

        glm::vec3 Nt;
        glm::vec3 Nb;

        if (fabs(a_Normal.x) > fabs(a_Normal.y))
        {
            Nt = glm::normalize(glm::vec3(a_Normal.z, 0, -a_Normal.x));
        }
        else
        {
            Nt = glm::normalize(glm::vec3(0, -a_Normal.z, a_Normal.y));
        }
        Nb = glm::cross(a_Normal, Nt);

        glm::vec3 randomDirection(
            hemisphere.x * Nb.x + hemisphere.y * a_Normal.x + hemisphere.z * Nt.x
            , hemisphere.x * Nb.y + hemisphere.y * a_Normal.y + hemisphere.z * Nt.y
            , hemisphere.x * Nb.z + hemisphere.y * a_Normal.z + hemisphere.z * Nt.z);

        return randomDirection;
    }
    */

	//Returns a random value between min (inclusive) and max (inclusive).
    float Random(float a_Min, float a_Max)
    {
        CROWN_PROFILE_FUNCTION();

		const float range = a_Max - a_Min;
        const float randomInRange = range * Random();
        return randomInRange + a_Min;
    }

    int Random(int a_Min, int a_Max)
    {
        CROWN_PROFILE_FUNCTION();

		const int range = a_Max - a_Min;
		const float randomInRange = static_cast<float>(range) * Random();
        return static_cast<int>(std::round(randomInRange)) + a_Min;
    }
}
