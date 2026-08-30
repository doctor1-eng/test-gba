#ifndef GUARD_CONFIG_MAP_TEST_H
#define GUARD_CONFIG_MAP_TEST_H

// Heart & Soul map-test build switch.
//
// Set to TRUE only by `make hns MAPTEST=1` (see root Makefile), which produces a separate,
// isolated ROM (heart-and-soul-map-test.gba, own build/ directory) dedicated to QA of the
// Cinnabar Gym/Mansion/Lab doors added to CinnabarIsland_hns. It changes nothing else and
// must never be enabled for the normal `hns` build.
//
// See MAP_TEST_README.md for what this build does and how to use it.
#ifndef HNS_MAP_TEST_BUILD
#define HNS_MAP_TEST_BUILD FALSE
#endif

#endif // GUARD_CONFIG_MAP_TEST_H
