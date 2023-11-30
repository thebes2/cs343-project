## CS343 Project

### Components:

Utility classes:
- [ ] Main
- [x] Config parser
- [x] Printer

Main classes:
- [ ] Student: depends on WATCardOffice (WATCard), Groupoff, NameServer, and VendingMachine
- [ ] WATCard: used by Student, depends on WATCardOffice, mainly just a data class/holder
- [ ] WATCardOffice: used by WATCard and Student, depends on Bank
- [x] Bank: used by WATCardOffice and Parent
- [x] Parent: isolated task, just depends on Bank
- [ ] VendingMachine: depends on WATCard, Truck, used by Student
- [x] BottlingPlant: another isolated task, just used by Truck
- [ ] Truck: depends on BottlingPlant and VendingMachine
- [ ] NameServer: used by both Student and VendingMachine

### Plan

Nov 29:
- Makefile
- Config parser & printer
- At least 1-2 of the isolated tasks (probably Parent and BottlingPlant)
