#ifndef _STATETYPES_H_
#define _STATETYPES_H_

#include <stdlib.h>
#include <string.h>
#include "hashtable_gs.h"
#define SECS_IN_DAY 86400

typedef int ServiceId;
typedef struct ServicePeriod ServicePeriod;
typedef struct ServiceCalendar ServiceCalendar;
typedef struct Timezone Timezone;
typedef struct TimezonePeriod TimezonePeriod;

struct ServiceCalendar {
    /* TripHops have service types, and the ServiceCalendar provides the correspondance between points in time and lists of service_ids.
    *  For example, A triphop that has a service_id attribute with the value "WKDY" will only run during service periods
    *  associated with the service_id "WKDY", which the service calendar will show corresponds roughly with weekdays. (Or, more literally, 
    *  the daytime periods of every day except two days on a seven day cycle.) As an optimization, triphop structs do not have a string attribute
    *  service_id, but an integer which corresponds to the service_id. Because most transit agencies use short strings for their service_ids,
    *  and talking about the service_id "weekday" is way easier than service_id 5, the service calendar has a lookup and reverse lookup table for string
    *  service_ids to integer service_ids.
    */
    
    ServicePeriod* head;
    
    int num_sids;
    struct hashtable* sid_str_to_int;
    char** sid_int_to_str;
} ; 

struct ServicePeriod {
  long begin_time; //the first second since the epoch on which the service period is in effect
  long end_time;   //first moment after the period; exclusive.
  int n_service_ids;
  ServiceId* service_ids;
  ServicePeriod* prev_period;
  ServicePeriod* next_period;
} ;

ServiceCalendar*
scNew( );

int
scAddServiceId( ServiceCalendar* this, char* service_id );

char*
scGetServiceIdString( ServiceCalendar* this, int service_id );

int
scGetServiceIdInt( ServiceCalendar* this, char* service_id );

int
scGetOrAddServiceIdInt( ServiceCalendar* this, char* service_id );

void
scAddPeriod( ServiceCalendar* this, ServicePeriod* period );

ServicePeriod*
scPeriodOfOrAfter( ServiceCalendar* this, long time );

ServicePeriod*
scPeriodOfOrBefore( ServiceCalendar* this, long time );

ServicePeriod*
scHead( ServiceCalendar* this );

void
scDestroy( ServiceCalendar* this );

ServicePeriod*
spNew( long begin_time, long end_time, int n_service_ids, ServiceId* service_ids );

void
spDestroyPeriod( ServicePeriod* this );

int
spPeriodHasServiceId( ServicePeriod* this, ServiceId service_id);

ServicePeriod*
spRewind( ServicePeriod* this );

ServicePeriod*
spFastForward( ServicePeriod* this );

void
spPrint( ServicePeriod* this ) ;

void
spPrintPeriod( ServicePeriod* this ) ;

inline long
spNormalizeTime( ServicePeriod* this, int timezone_offset, long time ) ;

struct Timezone {
    TimezonePeriod* head;
} ; 

struct TimezonePeriod {
  long begin_time; //the first second on which the service_period is valid
  long end_time;   //the last second on which the service_period is valid
  int utc_offset;
  TimezonePeriod* next_period;
} ;

Timezone*
tzNew( );

void
tzAddPeriod( Timezone* this, TimezonePeriod* period );

TimezonePeriod*
tzPeriodOf( Timezone* this, long time);

int
tzUtcOffset( Timezone* this, long time);

int
tzTimeSinceMidnight( Timezone* this, long time );

TimezonePeriod*
tzHead( Timezone* this );

void
tzDestroy( Timezone* this );

TimezonePeriod*
tzpNew( long begin_time, long end_time, int utc_offset );

void
tzpDestroy( TimezonePeriod* this );

int
tzpUtcOffset( TimezonePeriod* this );

int
tzpTimeSinceMidnight( TimezonePeriod* this, long time );

long
tzpBeginTime( TimezonePeriod* this );

long
tzpEndTime( TimezonePeriod* this );

TimezonePeriod*
tzpNextPeriod(TimezonePeriod* this);

#endif
