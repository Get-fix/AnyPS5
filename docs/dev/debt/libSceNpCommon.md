# libSceNpCommon technical debt

### Functional

- [sceNpCmpOnlineId](../../../core/libs/prx/libSceNpCommon/Export.cpp) (libSceNpCommon) - compares the first 16 characters of the two online ids, as [shadPS4](https://github.com/shadps4-emu/shadPS4/blob/81be949167d0060b08f26dc3e055a0b640d129c8/src/core/libraries/np/np_common.cpp) does: equal ids return 0, others `SCE_NP_UTIL_ERROR_NOT_MATCH` (0x80550609) and a null id `SCE_NP_ERROR_INVALID_ARGUMENT`. Not measured on a console
