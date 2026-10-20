# libSceNpTrophy technical debt

### Unknown function info

- [sceNpTrophyAbortHandle](../../../core/libs/prx/libSceNpTrophy/Export.cpp) (libSceNpTrophy) - imported by PS4 titles, which do not load without it, and added without an implementation: declared without parameters, every call throws

### Functional

- [sceNpTrophyGetGameInfo](../../../core/libs/prx/libSceNpTrophy/Export.cpp) (libSceNpTrophy) - the trophy configuration `TROPCONF.ESFM` is encrypted and not read, so the game details report the trophy count taken from the `TROPnnn.PNG` icons and the group count taken from the `GRnnn.PNG` icons of the trophy package; the per-grade counts are 0, the title and description are empty, and the progress percentage is unlocked trophies over all trophies instead of the console's grade-weighted points
- [sceNpTrophyGetGroupInfo](../../../core/libs/prx/libSceNpTrophy/Export.cpp) (libSceNpTrophy) - only the base game group (id -1) is answered, with the same counts as the game; an id past the `GRnnn.PNG` icons returns `INVALID_GROUP_ID`, and an existing group throws because the trophies of a group are listed only in the encrypted configuration
- [sceNpTrophyGetTrophyInfo](../../../core/libs/prx/libSceNpTrophy/Export.cpp) (libSceNpTrophy) - the grade is reported as unknown, the trophy is never hidden, the name and description are empty and the unlock timestamp is 0; a package with groups throws, since a trophy's group is listed only in the encrypted configuration
