#pragma once

// Base Types
#include "basetypes/BYTES.h"
#include "basetypes/FH20.h"
#include "basetypes/FH256.h"
#include "basetypes/FH32.h"
#include "basetypes/FH8.h"
#include "basetypes/IPADDRESS.h"
#include "basetypes/VALUE.h"
#include "configs/FORK.h"
#include "configs/SealEngine.h"

// Old blockchain tests didn't sort maps when exporting state/accounts
// Maps do autosort. To see minimum changes in the tests use this export order
enum class ExportOrder
{
    Default,
    OldStyle
};
