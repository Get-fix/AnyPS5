# libSceNpSignaling technical debt

### Unknown function info

- [sceNpSignalingDeactivateConnection](../../../core/libs/prx/libSceNpSignaling/Export.cpp) (libSceNpSignaling) - imported by PS4 titles, which do not load without it, and added without an implementation: declared without parameters, every call throws

### Functional

- [sceNpSignalingInitialize, sceNpSignalingTerminate](../../../core/libs/prx/libSceNpSignaling/Export.cpp) (libSceNpSignaling) - only the library state is kept, with the `ALREADY_INITIALIZED` and `NOT_INITIALIZED` errors of [shadPS4](https://github.com/shadps4-emu/shadPS4/blob/81be949167d0060b08f26dc3e055a0b640d129c8/src/core/libraries/np/np_signaling/np_signaling.cpp); the pool, thread priority, affinity and stack size are not used and no signaling thread starts, since no peer is reached. Not measured on a console
