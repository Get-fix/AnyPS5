# libSceNpUtility technical debt

### Unknown function info

- [sceNpLookupAbortRequest, sceNpLookupCreateAsyncRequest, sceNpLookupPollAsync](../../../core/libs/prx/libSceNpUtility/Export.cpp) (libSceNpUtility) - imported by PS4 titles, which do not load without them, and added without an implementation: declared without parameters, every call throws

### Functional

- [sceNpLookupCreateTitleCtx](../../../core/libs/prx/libSceNpUtility/Export.cpp) (libSceNpUtility) - a lookup context belongs to a signed-in user's NP id, and no user is signed in: a null NP id returns `SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT` (0x8055070C) and any other `SCE_NP_ERROR_SIGNED_OUT`, so no context is created. [shadPS4](https://github.com/shadps4-emu/shadPS4/blob/81be949167d0060b08f26dc3e055a0b640d129c8/src/core/libraries/np/np_utility/np_utility.cpp) creates one for any NP id; what the console returns for a signed-out user is not measured
