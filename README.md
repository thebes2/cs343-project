## CS343 Project: Spurious Wakeups

### Components:

Utility classes:
- [x] Main
- [x] Config parser
- [x] Printer

Main classes:
- [x] Student: depends on WATCardOffice (WATCard), Groupoff, NameServer, and VendingMachine
- [x] WATCard: used by Student, depends on WATCardOffice, mainly just a data class/holder
- [x] WATCardOffice: used by WATCard and Student, depends on Bank
- [x] Bank: used by WATCardOffice and Parent
- [x] Parent: isolated task, just depends on Bank
- [x] VendingMachine: depends on WATCard, Truck, used by Student
- [x] BottlingPlant: another isolated task, just used by Truck
- [x] Truck: depends on BottlingPlant and VendingMachine
- [x] NameServer: used by both Student and VendingMachine
