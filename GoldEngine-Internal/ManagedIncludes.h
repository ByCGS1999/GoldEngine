#pragma once

#pragma managed(push, on)

#include "Singleton.h"
#include <msclr/gcroot.h>
#include <msclr/lock.h>
#include <pplawait.h>
#include <ppltasks.h>
#include "EngineState.h"
#include "WinAPI.h"
#include "Cast.h"
#include "CastToNative.h"

// ATTRIBUTES \\

#include "ExecuteInEditModeAttribute.h"
#include "Instantiable.h"

#pragma managed(pop)