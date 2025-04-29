#include <iostream>
#include <algorithm>
#include "stdlib.h"
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"

struct PacketStats
{
    int ethPacketCount = 0;
    int ipv4PacketCount = 0;
    int ipv6PacketCount = 0;
    int tcpPacketCount = 0;
    int udpPacketCount = 0;
    int dnsPacketCount = 0;
    int httpPacketCount = 0;
    int sslPacketCount = 0;


  // deja a todas las variables listadas en 0
    void clear() { ethPacketCount = ipv4PacketCount = ipv6PacketCount = tcpPacketCount = udpPacketCount = dnsPacketCount = httpPacketCount = sslPacketCount = 0; }

    //El constructor se deja como predeterminado
    PacketStats() = default;

  //Toman valor las variables de cada paquete que llega
    void consumePacket(pcpp::Packet& packet)
    {
        if (packet.isPacketOfType(pcpp::Ethernet))
            ethPacketCount++;
        if (packet.isPacketOfType(pcpp::IPv4))
            ipv4PacketCount++;
        if (packet.isPacketOfType(pcpp::IPv6))
            ipv6PacketCount++;
        if (packet.isPacketOfType(pcpp::TCP))
            tcpPacketCount++;
        if (packet.isPacketOfType(pcpp::UDP))
            udpPacketCount++;
        if (packet.isPacketOfType(pcpp::DNS))
            dnsPacketCount++;
        if (packet.isPacketOfType(pcpp::HTTP))
            httpPacketCount++;
        if (packet.isPacketOfType(pcpp::SSL))
            sslPacketCount++;
    }

  //Muestra los resultados  del conteo en consola
    void printToConsole()
    {
        std::cout
            << "Ethernet packet count: " << ethPacketCount << std::endl
            << "IPv4 packet count:     " << ipv4PacketCount << std::endl
            << "IPv6 packet count:     " << ipv6PacketCount << std::endl
            << "TCP packet count:      " << tcpPacketCount << std::endl
            << "UDP packet count:      " << udpPacketCount << std::endl
            << "DNS packet count:      " << dnsPacketCount << std::endl
            << "HTTP packet count:     " << httpPacketCount << std::endl
            << "SSL packet count:      " << sslPacketCount << std::endl;
    }
};

//Esta es la funcion callback, la que cada vez que llega un paquete se llama esta funcion, esta solo funciona para metodos asincronios, metodos que no se ejecutan en el main.
//raw packet, el paquete en bruto sin procesar
static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{
    //extrae el obgeto stats desde cookie
    auto* stats = static_cast<PacketStats*>(cookie);

    //analiza el paquete en bruto
    pcpp::Packet parsedPacket(packet);

   //obtiene los stats desde packet
    stats->consumePacket(parsedPacket);
}


int main(int argc, char* argv[]){
//se cambio de tomar informacion de una IP a una interfaz del equipo en este caso any que toma todo el trafico que hay en linux wsl.
std::string interfaceName = "any";
auto* dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(interfaceName);
if (dev == nullptr){
	std::cerr << "No se encontro la interfaz'" << interfaceName << "'" << std::endl;
    	return 1;
}
  // Informacion de la interfaz
std::cout
    << "Interface info:" << std::endl
    << "   Interface name:        " << dev->getName() << std::endl
    << "   Interface description: " << dev->getDesc() << std::endl
    << "   MAC address:           " << dev->getMacAddress() << std::endl
    << "   Default gateway:       " << dev->getDefaultGateway() << std::endl
    << "   Interface MTU:         " << dev->getMtu() << std::endl;

if (!dev->getDnsServers().empty()){
    std::cout << "   DNS server:            " << dev->getDnsServers().front() << std::endl;
  }
  // open the device before start capturing/sending packets
if (!dev->open())
{
    std::cerr << "Cannot open device" << std::endl;
    return 1;
}
// crea el obgeto stats para instanciar la esttructura
	PacketStats stats;

  std::cout << std::endl << "Starting async capture..." << std::endl;

  //se eligio el modo de captura asincronica ya que entrega los resultados inmediatamente.
  //se inicia la captura asincronica se da la funcion callback y las stacks de esta se ejecuntan como la coockie
  
	dev->startCapture(onPacketArrives, &stats);

	// Duerme por 10 segundos el main miesyras se capturan y analizan los paquetes.
	pcpp::multiPlatformSleep(10);

	//cuando se despierta la fucnion main se detiene la capturad e paquetes
	dev->stopCapture();

	// muestra los resultados
	std::cout << "Results:" << std::endl;
	stats.printToConsole();

	// limpia los stats
	stats.clear();
}
