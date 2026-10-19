# libSceNpTrophy technical debt

### Unknown function info

- [sceNpTrophyAbortHandle](../../../core/libs/prx/libSceNpTrophy/Export.cpp) (libSceNpTrophy) - imported by PS4 titles, which do not load without it, and added without an implementation: declared without parameters, every call throws
