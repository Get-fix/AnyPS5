# libSceGnmDriver technical debt

### Silent stubs

- [sceGnmAreSubmitsAllowed](../../../core/libs/prx/libSceGnmDriver/Export.cpp) (libSceGnmDriver, PS4) - returns 1: submissions are never suspended here, because nothing on the host takes the GPU away from the title. shadPS4 returns 0 only while its submission lock is held.
- [sceGnmIsUserPaEnabled, sceGnmDriverCaptureInProgress, sceRazorIsLoaded](../../../core/libs/prx/libSceGnmDriver/Export.cpp) (libSceGnmDriver, PS4) - return false: the user performance analysis mode, GPU captures and the Razor GPU profiler belong to development tools and are not present, as shadPS4 also answers for retail firmware. A title only skips its tool hooks on these answers.

### Unknown function info

- [Command buffer writers](../../../core/libs/prx/libSceGnmDriver/Export.cpp) (libSceGnmDriver, PS4) - the packets, register offsets and reserved sizes the `sceGnm` writers emit follow [shadPS4](https://github.com/shadps4-emu/shadPS4/blob/main/src/core/libraries/gnmdriver/gnmdriver.cpp); they are not checked on a console. Submitted buffers run on the AGC executor: PS4 EOP, EOS and SURFACE_SYNC packets are lowered to RELEASE_MEM and ACQUIRE_MEM, constant engine buffers are merged into the draw buffer, the CP_DMA prefetch of the PS4 SDK is a NOP, and a flip reservation for an interrupt flip throws.

### Functional

- [Packet types](../../../core/libs/prx/libSceGnmDriver/Export.cpp) (libSceGnmDriver, PS4) - a type-2 dword in a draw or constant command buffer is stepped over as filler, and a type-0 packet (a register write) is stepped over with its payload instead of writing the register; a type-1 packet throws. AMD's command processors skip type-2 filler (Linux radeon pads rings with it), and the radeon command stream checker accepts types 0, 2 and 3. Gang Beasts (CUSA04865) submits a type-0 header when its own compute dispatch, through a 16-byte V# over an 8-byte `allocateFromCommandBuffer` block, stores 1 over the next packet's header after the title has rebuilt the buffer for the next frame (the queue trails the title by several milliseconds here).
