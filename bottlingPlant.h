#ifndef _BOTTLING_PLANT_H_
#define _BOTTLING_PLANT_H_


_Task BottlingPlant {
    Printer *printer;
    NameServer &nameServer;
    unsigned int numVendingMachines, maxShippedPerFlavour, maxStockPerFlavour, timeBetweenShipments, randomQuantity, numFlavours;
    bool isShutdown;
    void main();
  public:
    enum Flavours {Blues_Black_Cherry, Classic_Cream_Soda, Rock_Root_Beer, Jazz_Lime}; // flavours of soda (YOU DEFINE)
    _Event Shutdown {}; // shutdown plant
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[] );
};

#endif