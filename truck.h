#ifndef _TRUCK_H_
#define _TRUCK_H_
_Task Truck {
    Printer *printer;
    NameServer &nameServer;
    BottlingPlant & plant;;
    unsigned int numVendingMachines, maxStockPerFlavour;
    unsigned int *shipment;
    void main();
    bool empty(int []);
  public:
    Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
};
#endif