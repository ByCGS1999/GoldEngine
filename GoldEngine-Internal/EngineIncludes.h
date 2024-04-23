#pragma once
#include "Includes.h"

// CUSTOM RENDERERS \\

#include "VoxelRenderer.h"

// INCLUDE ENGINE CLASSES \\

#include "SceneFormat.h"
#include "Objects/BoundingBoxRenderer.h"
#include "Objects/CubeRenderer.h"
#include "Objects/GridRenderer.h"
#include "Objects/LightManager.h"
#include "Objects/ModelRenderer.h"
#include "Objects/PBRModelRenderer.h"
#include "Objects/Skybox.h"

// last class to be imported, it must access to all the components included \\

#include "Attribute.h"
#include "AttributeManager.h"

#include "Objects/Script.h"
#include "Objects/MeshRenderer.h"

#include "Objects/Daemon.h"
#include "Objects/Camera3D.h"