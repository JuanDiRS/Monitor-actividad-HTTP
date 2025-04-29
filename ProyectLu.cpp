#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <PcapLiveDeviceList.h>
#include <SystemUtils.h>
#include <memory>
#include "Packet.h"
#include "EthLayer.h"
#include "IPv4Layer.h"
#include "TcpLayer.h"
#include "HttpLayer.h"
#include <PcapFileDevice.h>

//obtiene el protocolo del paquete y lo convierte a string.
std::string getProtocolTypeAsString(pcpp::ProtocolType protocolType)
{
	switch (protocolType){
	case pcpp::Ethernet:
		return "Ethernet";
	case pcpp::IPv4:
		return "IPv4";
	case pcpp::TCP:
		return "TCP";
	case pcpp::HTTPRequest:
	case pcpp::HTTPResponse:
		return "HTTP";
	default:
		return "Unknown";
	}
}

//hace un conteo simple de todos paquetes capturados
struct PacketStats
{
    	int conteopkt = 0;

  //Se suma el contador por cada paquete que llega
    void consumePacket(const pcpp::Packet& packet){
        conteopkt ++;
    }

  //Muestra los resultados  del conteo en consola
    void printToConsole()
    {
	std::cout << "Fueron: " << conteopkt << " paquetes en total."<<std::endl;
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

	for (auto* curLayer = parsedPacket.getFirstLayer(); curLayer != nullptr; curLayer = curLayer->getNextLayer()){
    	std::cout
        << "Layer type: " << getProtocolTypeAsString(curLayer->getProtocol()) << std::endl // obtiene el tipo de layer
        << "Total data: " << curLayer->getDataLen() << " [bytes]; " // la longitud totalk del layer
        << "Layer data: " << curLayer->getHeaderLen() << " [bytes]; " // la longitud del header del layer
        << "Layer payload: " << curLayer->getLayerPayloadSize() << " [bytes]"// longitud total del layer precargado lo cual es la longitud total menos el head layer
        << std::endl;
	}
	auto* httpRequestLayer = parsedPacket.getLayerOfType<pcpp::HttpRequestLayer>();
    	if (httpRequestLayer != nullptr){
        	auto* hostField = httpRequestLayer->getFieldByName(PCPP_HTTP_HOST_FIELD);
        	if (hostField != nullptr)
            		std::cout << "HTTP host: " << hostField->getFieldValue() << std::endl;
        	auto* userAgentField = httpRequestLayer->getFieldByName(PCPP_HTTP_USER_AGENT_FIELD);
        	if (userAgentField != nullptr)
            		std::cout << "HTTP user-agent: " << userAgentField->getFieldValue() << std::endl;
        	auto* cookieField = httpRequestLayer->getFieldByName(PCPP_HTTP_COOKIE_FIELD);
        	if (cookieField != nullptr)
            		std::cout << "HTTP cookie: " << cookieField->getFieldValue() << std::endl;
    }
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
}
