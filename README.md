# DCM

##DCM: Distributed Cache Management

This cache library is created to solve the interprocess problem caused by slow IO in data-centric system.
The idea is not well advanced comparing to sort of P2P techonology, especially to de-centralized distributed system, however, 
on the demands of Lower latency, this libray is faster and efficient to communicate among parties in concurrent situation.

This DCM is invented with the help of [boost](www.boost.org) library boost::interprocess

All code is c++ code with unit testing cases.

## Support Containers type
1. Normal map
2. Unordered Map(hash map)
3. set/hashset
4. Multi-Map
 
## Features
1. Shared memory supports automatically sizing;auto size swap on live(need to do the swap online in next phase)
2. keys are user-defined customized, interfacing is opened.


## Performance Testing result

1. searial write/reading speed 
   around 1.2~2m insert/second in CentOS VM 6.7
   10 times faster than MongoDB and LevelDB
2. concurrent, 
   two threads concurrently writeing, hit 20k insert/second, 2~2.5 times faster than MongoDB and LevelDB



   
