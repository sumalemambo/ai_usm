// constants.h
#if !defined(CONSTANTS_H)
#define CONSTANTS_H 1

const int UNASSIGNED_ENTITY = -1;

static const int ALLOCATION_CONSTRAINT = 0;
static const int NONALLOCATION_CONSTRAINT = 1;
static const int ONEOF_CONSTRAINT = 2;
static const int CAPACITY_CONSTRAINT = 3;
static const int SAMEROOM_CONSTRAINT = 4;
static const int NOTSAMEROOM_CONSTRAINT = 5;
static const int NOTSHARING_CONSTRAINT = 6;
static const int ADJACENCY_CONSTRAINT = 7;
static const int NEARBY_CONSTRAINT = 8;
static const int AWAYFROM_CONSTRAINT = 9;

#endif
