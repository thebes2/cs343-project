## CS343 Project

### Components:

Utility classes:
- [ ] Main
- [ ] Config parser
- [ ] Printer

Main classes:
- [ ] Student: depends on WATCardOffice (WATCard), Groupoff, NameServer, and VendingMachine
- [ ] WATCard: used by Student, depends on WATCardOffice, mainly just a data class/holder
- [ ] WATCardOffice: used by WATCard and Student, depends on Bank
- [ ] Bank: used by WATCardOffice and Parent
- [ ] Parent: isolated task, just depends on Bank
- [ ] VendingMachine: depends on WATCard, Truck, used by Student
- [ ] BottlingPlant: another isolated task, just used by Truck
- [ ] Truck: depends on BottlingPlant and VendingMachine
- [ ] NameServer: used by both Student and VendingMachine
