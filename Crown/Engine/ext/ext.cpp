#include "CrownPCH.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef APIENTRY
	
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#undef APIENTRY

#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>
